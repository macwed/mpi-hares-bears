#include "mhb/algorithm/lamport_algo/lamport_queue_algorithm.hpp"

#include <algorithm>
#include <cstddef>
#include <sstream>

namespace mhb {
Party::Party(const PartyStart& ps)
  : party_leader_(ps.sender_ticket.pid), party_id_(ps.party_id), members_(ps.participants) {}

LamportQueueAlgorithm::LamportQueueAlgorithm(Pid pid, const config::Config& cfg) : my_pid_(pid), cfg_(cfg) {
  my_species_ = config::SpeciesOf(pid, cfg);
  my_weight_ = config::WeightOf(pid, cfg);
  reply_tracker_ = std::vector<char>(cfg.world_size, 0);
}

void LamportQueueAlgorithm::MarkReply(Pid p) {
  reply_tracker_[p] = true;
}

bool LamportQueueAlgorithm::AmLeader() {
  uint32_t f = 0;
      return global_queue_.GetFirstPid(f) && f == my_pid_;
}

bool LamportQueueAlgorithm::AllRepliesReceived() {
  auto it = std::find(reply_tracker_.begin(), reply_tracker_.end(), 0);
  return it == reply_tracker_.end();
}

void LamportQueueAlgorithm::ResetTracker() {
  reply_tracker_.assign(cfg_.world_size, 0);
}

void LamportQueueAlgorithm::GoToParty(const PartyStart& party_start, std::vector<Action>& actions) {
  current_state_.state = AppState::kInParty;
  const auto& group = party_start.participants;
  my_party_.emplace(party_start);
  pending_req_ticket_.reset();
  ResetTracker();
  global_queue_.Remove(group.data(), group.size());
  actions.push_back(Notify{NotifyType::kEnteredParty});
}

void LamportQueueAlgorithm::Recompute(std::vector<Action>& actions) {
  if (current_state_.state == AppState::kWaitingForReplies) {
    if (AllRepliesReceived()) {
      lamport_clock_.Tick();
      current_state_.state = AppState::kQueued;
    } else
      return;
  }

  if (current_state_.state == AppState::kQueued && AmLeader()) {
    auto group_opt = TryBuildParty();
    if (!group_opt) {
      actions.emplace_back(Log{0, "Policy rejected group, leader waiting"});
      return;
    }
    if (std::find(group_opt->begin(), group_opt->end(), my_pid_) == group_opt->end()) {
      actions.emplace_back(Log{0, "Leader pid missing from group, skipping PartyStart"});
      return;
    }
    lamport_clock_.Tick();
    const auto& group = PartyStart{MyTicket(), CreatePartyId(), *group_opt};
    {
      std::ostringstream oss;
      oss << "PartyStart: leader=" << my_pid_
          << " party_id=" << group.party_id
          << " size=" << group.participants.size()
          << " members=[";
      for (size_t i = 0; i < group.participants.size(); ++i) {
        if (i > 0) oss << ",";
        oss << group.participants[i];
      }
      oss << "]";
      actions.emplace_back(Log{0, oss.str()});
    }
    GoToParty(group, actions);
    actions.emplace_back(Broadcast{group});
  }
}

Ticket LamportQueueAlgorithm::MyTicket() {
  Ticket ticket = Ticket(my_pid_, lamport_clock_.Now());
  return ticket;
}

//na przychodzący request: aktualizuj zegar, dodaj do kolejki, zwróć vector akcji Send reply
std::vector<Action> LamportQueueAlgorithm::OnMsg(Pid pid, Request request_received) {
  std::vector<Action> actions;
  const auto& ticket = request_received.request_ticket;
  lamport_clock_.OnReceive(ticket.ts);

  //przychodzi spóźniony request od kogoś kto już jest w tym samym party
  //TODO sprawdzić edge case
  /*if (current_state_.state == AppState::kInParty) {
    if (std::any_of(my_party_->members_.begin(), my_party_->members_.end(),
      [&](uint32_t m){return m == pid;})) {
      return {};
    }
  }*/

  global_queue_.Insert(ticket);
  Reply reply{MyTicket(), ticket};
  actions.emplace_back(Send{pid, reply});
  Recompute(actions);
  return actions;
}

//na przychodzący reply: podbij zegar, aktualizuj listę requestów, sprawdź warunki do Queued
std::vector<Action> LamportQueueAlgorithm::OnMsg(Pid pid, Reply reply) {
  lamport_clock_.OnReceive(reply.sender_ticket.ts);
  if (current_state_.state != AppState::kWaitingForReplies) {
    return {};
  }
  std::vector<Action> actions;
  MarkReply(pid);
  Recompute(actions);
  return actions;
}

std::vector<Action> LamportQueueAlgorithm::OnMsg(Pid pid, PartyStart party_start_received) {
  lamport_clock_.OnReceive(party_start_received.sender_ticket.ts);
  std::vector<Action> actions;
  auto& members = party_start_received.participants;
  global_queue_.Remove(members.data(), members.size());
  auto it = std::find(members.begin(), members.end(), my_pid_);
  if (it != members.end()) {
    if (current_state_.state == AppState::kInParty) {
      //TODO: log błędu, nie powinienem dostać PartyStart, jeśli już jestem w party
    }
    {
      std::ostringstream oss;
      oss << "PartyStart received: leader=" << party_start_received.sender_ticket.pid
          << " party_id=" << party_start_received.party_id
          << " size=" << party_start_received.participants.size();
      actions.emplace_back(Log{0, oss.str()});
    }
    GoToParty(party_start_received, actions);
    return actions;
  }
  Recompute(actions);
  return actions;
}

void LamportQueueAlgorithm::OnEvent(const Local& ev, std::vector<Action>& actions) {
  lamport_clock_.Tick();
  if (ev.type == LocalEventType::kWantParty) {
    //Event WantParty powinien pojawiać się tylko w stanie idle
    if (current_state_.state == AppState::kIdle) {
      current_state_.state = AppState::kWaitingForReplies;
      ResetTracker();
      MarkReply(my_pid_);
      pending_req_ticket_.emplace(MyTicket());
      global_queue_.Insert(*pending_req_ticket_);
      Request msg{*pending_req_ticket_};
      actions.emplace_back(Broadcast{msg});
    }
  }
  if (ev.type == LocalEventType::kReleaseParty) {
    my_party_.reset();
    pending_req_ticket_.reset();
    ResetTracker();
    current_state_.state = AppState::kIdle;
    actions.emplace_back(Notify{NotifyType::kExitedParty});
  }
}

void LamportQueueAlgorithm::OnEvent(const Net& ev, std::vector<Action>& actions) {
  Pid pid = ev.from;
  std::vector<Action> temp_vec;
  std::visit([&](const auto& msg) {
    temp_vec = OnMsg(pid, msg);
  }, ev.msg);
  actions.insert(actions.end(), temp_vec.begin(), temp_vec.end());
}

std::vector<Action> LamportQueueAlgorithm::HandleEvent(const Event& event) {
  std::vector<Action> actions;
  std::visit([&](auto const& ev) {
    OnEvent(ev, actions);
  }, event);

  return actions;
}

std::optional<std::vector<Pid>> LamportQueueAlgorithm::TryBuildParty() {
  std::vector<Ticket> snap = global_queue_.Snapshot();
  std::vector<PartyMember> candidates;
  candidates.reserve(snap.size());
  for (auto& candidate : snap) {
    Pid pid = candidate.pid;
    candidates.push_back({pid, config::WeightOf(pid, cfg_)});
  }
  std::vector<Pid> group;
  if (!policy_.TryFormGroup(candidates, group)) {
    return std::nullopt;
  }
  return group;
}

}  // namespace mhb

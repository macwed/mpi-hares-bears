#ifndef MHB_ALGORITHM_LAMPORT_QUEUE_ALGORITHM_H_
#define MHB_ALGORITHM_LAMPORT_QUEUE_ALGORITHM_H_

#include <optional>

#include "lamport_clock.hpp"
#include "mhb/policy/lamport_policy/lamport_policy.hpp"
#include "mhb/algorithm/algorithm.hpp"
#include "mhb/algorithm/state.hpp"
#include "mhb/core/config.hpp"
#include "mhb/queue/request_queue.hpp"

namespace mhb {

struct Party {
  Party(const PartyStart& ps);
  Pid party_leader_;
  PartyId party_id_;
  std::vector<Pid> members_;
};

class LamportQueueAlgorithm : public IAlgorithm {
  public:
  LamportQueueAlgorithm(Pid pid, const config::Config& cfg);
  std::vector<Action> HandleEvent(const Event& event) override;

  private:
  LamportClock lamport_clock_;
  AlgoState current_state_;
  RequestQueue global_queue_;
  Pid my_pid_;
  Weight my_weight_;
  config::Species my_species_;
  std::optional<Party> my_party_ = std::nullopt;
  std::optional<Ticket> pending_req_ticket_ = std::nullopt;
  std::vector<char> reply_tracker_;
  const config::Config& cfg_;
  std::vector<PartyMember> candidates_;
  LamportPolicy policy_ = LamportPolicy(cfg_.party_capacity, cfg_.hare_weight, cfg_.bear_weight);
  PartyId party_counter_ = my_pid_ * cfg_.party_id_offset;

  Ticket MyTicket();
  void OnEvent(const Local& ev, std::vector<Action>& actions);
  void OnEvent(const Net& ev, std::vector<Action>& actions);
  std::vector<Action> OnMsg(Pid pid, Request request_received);
  std::vector<Action> OnMsg(Pid pid, Reply reply_received);
  std::vector<Action> OnMsg(Pid pid, PartyStart party_start_received);
  std::optional<std::vector<Pid>> TryBuildParty();
  void MarkReply(Pid p);
  void Recompute(std::vector<Action>& actions);
  void GoToParty(const PartyStart& party_start, std::vector<Action>& actions);
  void ResetTracker();
  bool AmLeader();
  bool AllRepliesReceived();
  PartyId CreatePartyId() {
    auto p = party_counter_ + lamport_clock_.Now();
    party_counter_++;
    return p;
  }
};
}  // namespace mhb

#endif  // MHB_ALGORITHM_LAMPORT_QUEUE_ALGORITHM_H_

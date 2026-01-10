#include "mhb/queue/request_queue.hpp"

namespace mhb {
void RequestQueue::Insert(const Ticket& ticket) {
  //wstaw ticket do queue_ oraz iterator set do pid_to_ticket
  auto it_map = pid_to_ticket_.find(ticket.pid);
  if (it_map != pid_to_ticket_.end()) {
    queue_.erase(it_map -> second);
    pid_to_ticket_.erase(it_map);
  }

  auto [it, inserted] = queue_.insert(ticket);
  if (inserted) {
    pid_to_ticket_[it->pid] = it;
  }
}

void RequestQueue::Remove(Pid pid) {
  auto it_map = pid_to_ticket_.find(pid); //znajdź iterator seta po Pid
  if (it_map == pid_to_ticket_.end()) return;
  queue_.erase(it_map->second); //wydajne usuwanie po teratorze
  pid_to_ticket_.erase(it_map);
}

void RequestQueue::Remove(const Pid* pids, std::size_t num_pids) {
  for (std::size_t index = 0; index < num_pids; ++index) {
    Remove(pids[index]);
  }
}

bool RequestQueue::GetFirstPid(Pid& f) const {
  if (queue_.empty()) return false;
  f = queue_.begin() -> pid;
  return true;
}

std::vector<Ticket>RequestQueue::Snapshot() const {
  std::vector<Ticket> snapshot;
  snapshot.reserve(queue_.size());
  for (const auto& q : queue_) {
    snapshot.emplace_back(q);
  }
  return snapshot;
}

}  // namespace mhb

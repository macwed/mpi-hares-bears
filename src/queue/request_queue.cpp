#include "mhb/queue/request_queue.hpp"

namespace mhb {
void RequestQueue::Insert(const Ticket& ticket) {
  queue_.insert(ticket);
}

void RequestQueue::Remove(Pid pid) {
  for (auto it = queue_.begin(); it != queue_.end();) {
    if (it->pid == pid) {
      it = queue_.erase(it);
    } else {
      ++it;
    }
  }
}

void RequestQueue::Remove(const Pid* pids, std::size_t num_pids) {
  for (std::size_t index = 0; index < num_pids; ++index) {
    for (auto it = queue_.begin(); it != queue_.end();) {
      if (it->pid == pids[index]) {
        it = queue_.erase(it);
      } else {
        ++it;
      }
    }
  }
}

std::vector<Ticket> RequestQueue::AboveMe(const Ticket& my_ticket) const {
  auto it = queue_.lower_bound(my_ticket);
  return std::vector<Ticket>(queue_.begin(), it);
}
}  // namespace mhb

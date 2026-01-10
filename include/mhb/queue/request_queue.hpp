#ifndef MHB_QUEUE_REQUEST_QUEUE_H_
#define MHB_QUEUE_REQUEST_QUEUE_H_

#include <set>
#include <unordered_map>
#include <vector>

#include "mhb/core/ticket.hpp"
#include "mhb/core/types.hpp"

namespace mhb {

class RequestQueue {
 public:
  void Insert(const Ticket& ticket);
  void Remove(Pid pid);
  void Remove(const Pid* pids, std::size_t num_pids);
  bool GetFirstPid(Pid& f) const;
  std::vector<Ticket> Snapshot() const;

 private : std::set<Ticket> queue_;
  std::unordered_map<Pid, std::set<Ticket>::iterator> pid_to_ticket_;
};
}  // namespace mhb

#endif  // MHB_QUEUE_REQUEST_QUEUE_H_

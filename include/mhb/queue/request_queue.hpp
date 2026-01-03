#ifndef MHB_QUEUE_REQUEST_QUEUE_H_
#define MHB_QUEUE_REQUEST_QUEUE_H_

#include <cstddef>
#include <set>
#include <vector>

#include "mhb/core/ticket.hpp"

namespace mhb {
class RequestQueue {
 public:
  void Insert(const Ticket& ticket);
  void Remove(Pid pid);
  void Remove(const Pid* pids, std::size_t num_pids);
  std::vector<Ticket> AboveMe(const Ticket& my_ticket) const;

 private:
  std::set<Ticket> queue_;
};
}  // namespace mhb

#endif  // MHB_QUEUE_REQUEST_QUEUE_H_

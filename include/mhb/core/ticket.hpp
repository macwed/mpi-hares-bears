#ifndef MHB_CORE_TICKET_H_
#define MHB_CORE_TICKET_H_

#include "mhb/core/types.hpp"

namespace mhb {

struct Ticket {
  Pid pid{};
  Timestamp timestamp{};
};

bool operator<(const Ticket& lhs, const Ticket& rhs);
bool operator>(const Ticket& lhs, const Ticket& rhs);
}  // namespace mhb

#endif  // MHB_CORE_TICKET_H_

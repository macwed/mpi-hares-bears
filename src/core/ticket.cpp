#include "mhb/core/ticket.hpp"

namespace mhb {
bool operator<(const Ticket& lhs, const Ticket& rhs) {
  if (lhs.ts != rhs.ts) {
    return lhs.ts < rhs.ts;
  }
  return lhs.pid < rhs.pid;
}

bool operator>(const Ticket& lhs, const Ticket& rhs) {
  if (lhs.ts != rhs.ts) {
    return lhs.ts > rhs.ts;
  }
  return lhs.pid > rhs.pid;
}
}  // namespace mhb

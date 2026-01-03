#include "mhb/core/ticket.hpp"

namespace mhb {
bool operator<(const Ticket& lhs, const Ticket& rhs) {
  if (lhs.timestamp != rhs.timestamp) {
    return lhs.timestamp < rhs.timestamp;
  }
  return lhs.pid < rhs.pid;
}

bool operator>(const Ticket& lhs, const Ticket& rhs) {
  if (lhs.timestamp != rhs.timestamp) {
    return lhs.timestamp > rhs.timestamp;
  }
  return lhs.pid > rhs.pid;
}
}  // namespace mhb

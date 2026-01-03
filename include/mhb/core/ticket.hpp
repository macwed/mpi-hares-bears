#ifndef MHB_CORE_TICKET_H_
#define MHB_CORE_TICKET_H_

#include <cstdint>

#include "mhb/core/types.hpp"

namespace mhb {
enum class Animal : uint8_t {
  kHare = 0,
  kBear = 1
};

struct Ticket {
  Pid pid{};
  Timestamp timestamp{};
  Animal animal{};
};

bool operator<(const Ticket& lhs, const Ticket& rhs);
bool operator>(const Ticket& lhs, const Ticket& rhs);
}  // namespace mhb

#endif  // MHB_CORE_TICKET_H_

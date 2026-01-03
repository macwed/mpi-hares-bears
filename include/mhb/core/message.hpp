#ifndef MHB_CORE_MESSAGE_H_
#define MHB_CORE_MESSAGE_H_

#include <array>

#include "mhb/core/config.hpp"
#include "mhb/core/ticket.hpp"

namespace mhb {
enum class MessageType {
  kRequest,
  kReply,
  kPartyStart
};

struct Message {
  MessageType type{};
  Ticket ticket{};
  std::array<Pid, kMaxGroupSize> members{};
  uint32_t num_members{};
};
}  // namespace mhb

#endif  // MHB_CORE_MESSAGE_H_

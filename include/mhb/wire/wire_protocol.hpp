#ifndef MPI_HARES_BEARS_WIRE_PROTOCOL_HPP
#define MPI_HARES_BEARS_WIRE_PROTOCOL_HPP

#include <cstdint>
#include <optional>
#include <span>
#include <vector>

#include "mhb/core/message.hpp"

namespace mhb::wire {

std::vector<std::uint8_t> Serialize(const Message& msg);
std::optional<Message> Deserialize(std::span<const std::uint8_t> data);
}


#endif  // MPI_HARES_BEARS_WIRE_PROTOCOL_HPP

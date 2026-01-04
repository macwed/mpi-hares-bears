//
// Created by maciej on 4.01.2026.
//

#ifndef MPI_HARES_BEARS_ACTION_HPP
#define MPI_HARES_BEARS_ACTION_HPP

#include "message.hpp"
#include "types.hpp"

#include <variant>

namespace mhb {
namespace action {

struct Send {};
struct Broadcast {};
struct Log {};

}
}

#endif  // MPI_HARES_BEARS_ACTION_HPP

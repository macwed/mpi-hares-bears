//
// Created by maciej on 11.01.2026.
//

#ifndef MPI_HARES_BEARS_EVENT_HPP
#define MPI_HARES_BEARS_EVENT_HPP
#include "message.hpp"
#include "types.hpp"

namespace mhb {

enum class LocalEventType {
  kWantParty,
  kReleaseParty
};

struct Local {
  LocalEventType type;
};

struct Net {
  Pid from;
  Message msg;
};

using Event = std::variant<Local, Net>;

}

#endif  // MPI_HARES_BEARS_EVENT_HPP

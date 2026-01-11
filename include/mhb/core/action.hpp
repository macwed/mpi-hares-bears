//
// Created by maciej on 4.01.2026.
//

#ifndef MPI_HARES_BEARS_ACTION_HPP
#define MPI_HARES_BEARS_ACTION_HPP

#include <string>
#include <variant>

#include "message.hpp"
#include "types.hpp"

//akcje opisują decyzje podejmowane przez algorytm

namespace mhb {

enum class NotifyType {
  kEnteredParty,
  kExitedParty
};

struct Send {
  Pid to;
  Message msg;
};

struct Broadcast {
  Message msg;
};

struct Notify {
  NotifyType type;
};

struct Log {
  LogLevel level;
  std::string text;
};

using Action = std::variant<Send, Broadcast, Notify, Log>;

}

#endif  // MPI_HARES_BEARS_ACTION_HPP

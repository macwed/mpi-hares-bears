#ifndef MPI_HARES_BEARS_STATE_HPP
#define MPI_HARES_BEARS_STATE_HPP
#include <optional>

#include "mhb/core/ticket.hpp"
#include "mhb/core/types.hpp"

namespace mhb {

enum class AppState {
  kIdle,              //proces czeka, symuluje własną prywatną pracę
  kWaitingForReplies, //proces chce do party, wstawił swój Pid do kolejki, wysłał req, czeka na rep
  kQueued,            //proces otrzymał komplet rep, jeśli nie może stworzyć party czeka w tym stanie
  kInParty            //proces utworzył imprezę, wysłał bc PartyStar
};

struct AlgoFlags {
  bool am_i_leader = false;
};

struct AlgoState {
  AppState state = AppState::kIdle;
  AlgoFlags flags;
  std::optional<Ticket> my_request;
  std::optional<PartyId> current_party;
};

}
#endif  // MPI_HARES_BEARS_STATE_HPP

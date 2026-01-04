#ifndef MHB_CORE_MESSAGE_H_
#define MHB_CORE_MESSAGE_H_

#include "mhb/core/ticket.hpp"

#include <variant>
#include <vector>


namespace mhb {

struct Request {Ticket request_ticket;}; // req z tiketem pid ts requestującego
struct Reply {Ticket sender_ticket; Ticket request_ticket;}; //rep z pid ts odpowiadającego i reqticket na który udziela odp
struct PartyStart {Ticket sender_ticket; uint64_t party_id; std::vector<Pid> participants;}; //pid ts startującego imprezę, id party (głównie debug), pidy uczestników

using Message = std::variant<Request, Reply, PartyStart>;

}  // namespace mhb

#endif  // MHB_CORE_MESSAGE_H_

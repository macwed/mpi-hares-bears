#ifndef MHB_TRANSPORT_MPI_TRANSPORT_H_
#define MHB_TRANSPORT_MPI_TRANSPORT_H_

#include "transport.hpp"

namespace mhb {
class MpiTransport final : public Transport {
  public:
  MpiTransport();
  void send(Pid to, Message& msg) const override;
  void broadcast(Message& msg) const override;
  bool poll(Pid& from, Message& out) const override;
};
}  // namespace mhb

#endif  // MHB_TRANSPORT_MPI_TRANSPORT_H_

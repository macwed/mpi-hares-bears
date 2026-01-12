#ifndef MHB_TRANSPORT_MPI_TRANSPORT_H_
#define MHB_TRANSPORT_MPI_TRANSPORT_H_

#include "ITransport.hpp"

namespace mhb {
class MpiTransport final : public ITransport {
  public:
  MpiTransport();
  void Send(Pid to, const Message& msg) override;
  void Broadcast(const Message& msg) override;
  std::vector<Net> Poll() override;
  Pid Self() const override;
  uint32_t WorldSize() const override;

  private:
  Pid self_{};
  uint32_t world_size_{};
};
}  // namespace mhb

#endif  // MHB_TRANSPORT_MPI_TRANSPORT_H_

#ifndef MHB_TRANSPORT_TRANSPORT_H_
#define MHB_TRANSPORT_TRANSPORT_H_

#include <vector>

#include "mhb/core/event.hpp"
#include "mhb/core/message.hpp"
#include "mhb/core/types.hpp"

namespace mhb {
class ITransport {
 public:
  virtual ~ITransport() = default;
  virtual void Send(Pid to, const Message& msg) = 0;
  virtual void Broadcast(const Message& msg) = 0;
  virtual std::vector<Net> Poll() = 0;
  virtual Pid Self() const = 0;
  virtual uint32_t WorldSize() const = 0;
};
}  // namespace mhb

#endif  // MHB_TRANSPORT_TRANSPORT_H_

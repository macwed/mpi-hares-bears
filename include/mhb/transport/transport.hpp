#ifndef MHB_TRANSPORT_TRANSPORT_H_
#define MHB_TRANSPORT_TRANSPORT_H_

#include "mhb/core/types.hpp"
#include "mhb/core/message.hpp"

namespace mhb {
class Transport {
 public:
  virtual ~Transport() = default;
  virtual void send(Pid to, Message& msg) const = 0;
  virtual void broadcast(Message& msg) const = 0;
  virtual bool poll(Pid& from, Message& out) const = 0;
};
}  // namespace mhb

#endif  // MHB_TRANSPORT_TRANSPORT_H_

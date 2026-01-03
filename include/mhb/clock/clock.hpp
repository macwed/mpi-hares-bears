#ifndef MHB_CLOCK_CLOCK_H_
#define MHB_CLOCK_CLOCK_H_

#include "mhb/core/types.hpp"

namespace mhb {
class Clock {
 public:
  virtual ~Clock() = default;
  virtual Timestamp Tick() = 0;
  virtual void OnReceive(Timestamp remote_time) = 0;
  virtual Timestamp Now() const = 0;
};
}  // namespace mhb

#endif  // MHB_CLOCK_CLOCK_H_

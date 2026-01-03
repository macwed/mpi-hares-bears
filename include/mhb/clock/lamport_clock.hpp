#ifndef MHB_CLOCK_LAMPORT_CLOCK_H_
#define MHB_CLOCK_LAMPORT_CLOCK_H_

#include "mhb/clock/clock.hpp"

namespace mhb {
class LamportClock final : public Clock {
 public:
  LamportClock();

  Timestamp Tick() override;
  void OnReceive(Timestamp remote_time) override;
  Timestamp Now() const override;

 private:
  Timestamp time_{};
};
}  // namespace mhb

#endif  // MHB_CLOCK_LAMPORT_CLOCK_H_

#include "mhb/clock/lamport_clock.hpp"

#include <algorithm>

namespace mhb {
LamportClock::LamportClock() : time_(0) {}

Timestamp LamportClock::Tick() {
  return ++time_;
}

void LamportClock::OnReceive(Timestamp remote_time) {
  time_ = std::max(time_, remote_time) + 1;
}

Timestamp LamportClock::Now() const {
  return time_;
}
}  // namespace mhb

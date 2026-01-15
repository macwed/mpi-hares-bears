#ifndef MPI_HARES_BEARS_LOCAL_THREAD_HPP
#define MPI_HARES_BEARS_LOCAL_THREAD_HPP

#include <chrono>
#include <vector>

#include "mhb/core/action.hpp"
#include "mhb/core/event.hpp"

namespace mhb {

constexpr double kPartyTime = 500;

class LocalThread {
 public:
  std::vector<Local> Poll() {
    std::vector<Local> out;
    if (should_want_ && ShouldWantParty()) {
      out.push_back(Local{LocalEventType::kWantParty});
      should_want_ = false;
    }

    if (ShouldReleaseParty()) {
      out.push_back(Local{LocalEventType::kReleaseParty});
    }

    return out;
  }

  void OnNotify(NotifyType type) {
    current_notification_ = type;
    if (type == NotifyType::kEnteredParty) {
      time_mark_ = std::chrono::steady_clock::now();
    } else if (type == NotifyType::kExitedParty) {
      should_want_ = true;
    }
  }

 private:
  bool should_want_ = true;
  std::chrono::time_point<std::chrono::steady_clock> time_mark_ =
      std::chrono::steady_clock::now();
  NotifyType current_notification_ = NotifyType::kExitedParty;

  bool ShouldWantParty() const {
    return current_notification_ != NotifyType::kEnteredParty;
  }

  bool ShouldReleaseParty() const {
    if (current_notification_ != NotifyType::kEnteredParty) {
      return false;
    }
    const auto elapsed = std::chrono::steady_clock::now() - time_mark_;
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    return static_cast<double>(ms) > kPartyTime;
  }
};

}

#endif  // MPI_HARES_BEARS_LOCAL_THREAD_HPP

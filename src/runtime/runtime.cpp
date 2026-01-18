#include "mhb/runtime/runtime.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <type_traits>
#include <utility>

#include "mhb/core/event.hpp"

namespace mhb {
Runtime::Runtime(std::unique_ptr<ITransport> transport, std::unique_ptr<IAlgorithm> algorithm,
                 LocalThread local_src)
    : transport_(std::move(transport)), algorithm_(std::move(algorithm)),
      local_src_(std::move(local_src)) {}

void Runtime::Run() {
  running_ = true;
  while (running_) {
    for (auto& net : transport_->Poll()) {
      Event ev = Net{net.from, std::move(net.msg)};
      DrainActions(algorithm_->HandleEvent(ev));
    }

    for (auto& loc : local_src_.Poll()) {
      Event ev = Local{loc.type};
      DrainActions(algorithm_->HandleEvent(ev));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void Runtime::Stop() {
  running_ = false;
}

void Runtime::DrainActions(std::vector<Action> actions) {
  for (auto& a : actions) {
    Execute(a);
  }
}

void Runtime::Execute(const Action& action) {
  std::visit([&](const auto& a) {
    using T = std::decay_t<decltype(a)>;
    if constexpr (std::is_same_v<T, Send>) {
      transport_->Send(a.to, a.msg);
    } else if constexpr (std::is_same_v<T, Broadcast>) {
      transport_->Broadcast(a.msg);
    } else if constexpr (std::is_same_v<T, Notify>) {
      OnNotify(a.type);
    } else if constexpr (std::is_same_v<T, Log>) {
      LogLine(a.text);
    }
  }, action);
}

void Runtime::OnNotify(NotifyType type) {
  local_src_.OnNotify(type);
  switch (type) {
    case NotifyType::kEnteredParty:
      LogLine("Notify: entered party");
      break;
    case NotifyType::kExitedParty:
      LogLine("Notify: exited party");
      break;
  }
}

void Runtime::LogLine(const std::string& text) {
  std::cout << "[pid " << transport_->Self() << "] " << text << "\n";
  std::cout.flush();
}
}  // namespace mhb

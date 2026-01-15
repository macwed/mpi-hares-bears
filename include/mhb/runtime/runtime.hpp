#ifndef MHB_RUNTIME_RUNTIME_H_
#define MHB_RUNTIME_RUNTIME_H_

#include <memory>
#include <string>
#include <vector>

#include "mhb/algorithm/algorithm.hpp"
#include "mhb/core/action.hpp"
#include "mhb/transport/ITransport.hpp"
#include "mhb/local/local_thread.hpp"

namespace mhb {
class Runtime {
 public:
  Runtime(std::unique_ptr<ITransport> transport,
          std::unique_ptr<IAlgorithm> algorithm,
          LocalThread local_src);
  void Run();
  void Stop();

 private:
  void DrainActions(std::vector<Action> actions);
  void Execute(const Action& action);
  void OnNotify(NotifyType type);
  void LogLine(const std::string& text);

  std::unique_ptr<ITransport> transport_;
  std::unique_ptr<IAlgorithm> algorithm_;
  LocalThread local_src_;
  bool running_ = true;
};

}  // namespace mhb

#endif  // MHB_RUNTIME_RUNTIME_H_

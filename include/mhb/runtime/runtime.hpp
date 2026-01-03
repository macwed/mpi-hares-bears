#ifndef MHB_RUNTIME_RUNTIME_H_
#define MHB_RUNTIME_RUNTIME_H_

#include <memory>

#include "mhb/algorithm/algorithm.hpp"
#include "mhb/transport/transport.hpp"

namespace mhb {
class Runtime {
 public:
  Runtime(std::unique_ptr<Transport> transport,
          std::unique_ptr<Algorithm> algorithm);
  void Run();

 private:
  std::unique_ptr<Transport> transport_;
  std::unique_ptr<Algorithm> algorithm_;
};
}  // namespace mhb

#endif  // MHB_RUNTIME_RUNTIME_H_

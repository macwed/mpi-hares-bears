#ifndef MHB_ALGORITHM_ALGORITHM_H_
#define MHB_ALGORITHM_ALGORITHM_H_

#include "mhb/core/action.hpp"
#include "mhb/core/message.hpp"
#include "mhb/core/event.hpp"

#include <vector>

namespace mhb {

class IAlgorithm {
 public:
  virtual ~IAlgorithm() = default;
  virtual std::vector<Action> HandleEvent(const Event& event) = 0;
};

}  // namespace mhb

#endif  // MHB_ALGORITHM_ALGORITHM_H_

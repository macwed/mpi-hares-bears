#ifndef MHB_CORE_CONFIG_H_
#define MHB_CORE_CONFIG_H_

#include <cstdint>

namespace mhb {
constexpr uint32_t kMaxGroupSize = 16;
constexpr uint32_t kDefaultHares = 32;
constexpr uint32_t kDefaultBears = 8;

struct Config {
  uint32_t group_size = kMaxGroupSize;
  uint32_t hares = kDefaultHares;
  uint32_t bears = kDefaultBears;
};
}  // namespace mhb

#endif  // MHB_CORE_CONFIG_H_

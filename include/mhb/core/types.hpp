#ifndef MHB_CORE_TYPES_H_
#define MHB_CORE_TYPES_H_

#include <cstdint>

namespace mhb {
using Timestamp = uint64_t;
using Pid = uint32_t;
using PartyId = uint64_t;
using Weight = uint8_t;
using LogLevel = uint8_t;

enum class Species : Weight {
  kHare = 1,
  kBear = 4
};

}  // namespace mhb

#endif  // MHB_CORE_TYPES_H_

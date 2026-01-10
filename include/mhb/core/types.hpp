#ifndef MHB_CORE_TYPES_H_
#define MHB_CORE_TYPES_H_

#include <cstdint>

namespace mhb {
using Timestamp = uint64_t;
using Pid = uint32_t;
using PartyId = uint32_t;
using Weight = uint32_t;
using LogLevel = uint8_t;

struct PartyMember {
  Pid pid;
  Weight w;
};

}  // namespace mhb

#endif  // MHB_CORE_TYPES_H_

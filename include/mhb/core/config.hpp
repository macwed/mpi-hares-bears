#ifndef MHB_CORE_CONFIG_H_
#define MHB_CORE_CONFIG_H_

#include <cstdint>

#include "types.hpp"


namespace mhb::config {
constexpr uint32_t kDefaultWorldSize = 18;
constexpr uint32_t kDefaultPartyCapacity = 6;
constexpr uint32_t kDefaultHares = 13;
constexpr uint32_t kDefaultBears = kDefaultWorldSize - kDefaultHares;
constexpr uint32_t kDefaultPartyIdOffset = 10'000'000;
constexpr uint8_t kDefaultBearWeight = 4;
constexpr uint8_t kDefaultHareWeight = 1;

enum class Species : uint8_t {
  kHare,
  kBear,
};

struct Config {
  uint32_t world_size = kDefaultWorldSize;
  uint32_t party_capacity = kDefaultPartyCapacity;
  uint32_t num_hares = kDefaultHares;
  uint32_t num_bears = kDefaultBears;
  uint32_t party_id_offset = kDefaultPartyIdOffset;
  uint8_t bear_weight = kDefaultBearWeight;
  uint8_t hare_weight = kDefaultHareWeight;

  uint32_t run_seconds = 0;      // 0 = nieograniczony czas działania
  uint32_t max_parties = 0;      // limit imprez
};

constexpr Species SpeciesOf(Pid p, const Config& cfg) {
  if (p < cfg.num_hares) {
    return Species::kHare;
  }
  return Species::kBear;
}

constexpr Weight WeightOf(Pid p, const Config& cfg) {
  if (p < cfg.num_hares) {
    return cfg.hare_weight;
  }
  return cfg.bear_weight;
}

}

#endif  // MHB_CORE_CONFIG_H_

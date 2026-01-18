//
// Created by maciej on 14.01.2026.
//

#ifndef MPI_HARES_BEARS_LAMPORT_POLICY_HPP
#define MPI_HARES_BEARS_LAMPORT_POLICY_HPP

#include <cstdint>
#include <vector>

#include "mhb/core/types.hpp"
#include "mhb/policy/group_policy.hpp"

namespace mhb {

class LamportPolicy : public GroupPolicy {
  public:
  explicit LamportPolicy(uint32_t max_capacity, uint8_t hare_weight, uint8_t bear_weight);

  bool TryFormGroup(std::vector<PartyMember> candidates, std::vector<Pid>& group) override;

  private:
  uint32_t capacity_;
  uint8_t hare_weight_;
  uint8_t bear_weight_;
};

}

#endif  // MPI_HARES_BEARS_LAMPORT_POLICY_HPP

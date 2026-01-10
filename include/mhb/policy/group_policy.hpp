#ifndef MHB_POLICY_GROUP_POLICY_H_
#define MHB_POLICY_GROUP_POLICY_H_

#include <span>
#include <vector>

#include "mhb/core/types.hpp"

namespace mhb {

class GroupPolicy {
  public:
  virtual ~GroupPolicy() = default;

  virtual bool TryFormGroup(std::span<const PartyMember> candidates, std::vector<Pid>& group) = 0;
};

}  // namespace mhb

#endif  // MHB_POLICY_GROUP_POLICY_H_

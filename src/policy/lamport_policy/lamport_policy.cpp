#include "mhb/policy/lamport_policy/lamport_policy.hpp"

#include <span>
#include <unordered_set>
#include <vector>

#include "mhb/core/types.hpp"

namespace mhb {
LamportPolicy::LamportPolicy(uint32_t max_capacity, uint8_t hare_weight, uint8_t bear_weight)
        : capacity_(max_capacity), hare_weight_(hare_weight),
          bear_weight_(bear_weight) {}

bool LamportPolicy::TryFormGroup(std::span<const PartyMember> candidates, std::vector<Pid>& group) {
  group.clear();
  if (candidates.empty() || capacity_ == 0) return false;
  //przyjmuję wagę 1 dla zająca wg założeń projektowych
  //żeby nie robić całej skomplikowanej heurystyki dla dowolnego problemu plecakowego
  if (hare_weight_ != 1) {
    //TODO log nieobsługiwanego typu/wagi?
    return false;
  }

  const auto& leader = candidates[0];
  if (leader.w > capacity_) return false;
  uint32_t remaining = capacity_ - leader.w;
  bool hare_in_group = (leader.w == hare_weight_); //czy leader to zając?
  //zawsze leader będzie dodany pierwszy, niezależnie czy zając czy niedzwiedz

  //zliczam zające i niedźwiedzie wśród kandydatów
  std::vector<Pid> hares;
  std::vector<Pid> bears;
  hares.reserve(candidates.size());
  bears.reserve(candidates.size());

  for (std::size_t i = 1; i < candidates.size(); ++i) {
    const auto& c = candidates[i];
    if (c.w == hare_weight_) hares.push_back(c.pid);
    else if (c.w == bear_weight_) bears.push_back(c.pid);
    else {
      //TODO log nieobsługiwanego typu/wagi ?
    }
  }

  if (!hare_in_group && hares.empty()) return false; //nie da się uformować grupy, brak zająców

  uint32_t place_for_hare = hare_in_group ? 0u : 1u;
  if (remaining < place_for_hare) return false;

  //sprawdzam ile maksymalnie niedźwiedzi można zmieścić na polanie
  //jeśli wśród kandydatów mniej niedźwiedzi -> weź mniejszą liczbę
  uint32_t max_bears_num = remaining / bear_weight_;
  uint32_t b_max = std::min<uint32_t>(max_bears_num, static_cast<uint32_t>(bears.size()));

  uint32_t bears_to_take = 0;
  bool feasible = false;

  //sprawdzam ile max niedźwiedzi na polanie i czy wystarczy w ogóle zająców do dopełnienia
  for (uint32_t b = b_max + 1; b-- > 0; ) {
    if (b > remaining / bear_weight_) continue; //guard przed underflow (trochę redundant)

    uint32_t rem2 = remaining - b * bear_weight_;
    if (rem2 < place_for_hare) continue;
    if (rem2 > hares.size()) continue;
    bears_to_take = b;
    feasible = true;
    break;
  }
  if (!feasible) return false;

  std::vector<Pid> chosen;
  chosen.reserve(candidates.size());
  std::unordered_set<Pid> used;
  used.reserve(candidates.size());

  auto add_pid = [&](Pid pid) { //lambda sprawdza czy pid już w secie used i dodaje do chosen
    if (used.insert(pid).second) {
      chosen.push_back(pid);
      return true;
    }
    return false;
  };
  add_pid(leader.pid);

  uint32_t rem = remaining;
  for (uint32_t i = 0; i < bears_to_take; ++i) {
    const Pid pid = bears[i];
    add_pid(pid);
    rem -= bear_weight_;
  }

  //dopełniam zającami
  const uint32_t hares_needed = rem;
  uint32_t taken_hares = 0;
  for (Pid pid : hares) {
    if (taken_hares == hares_needed) break;
    if (add_pid(pid)) {
      ++taken_hares;
      hare_in_group = true;
    }
  }

  if (taken_hares != hares_needed) return false;
  if (!hare_in_group) return false;

  /* sanity check do debugu
   *uint32_t total = leader.w + bears_to_take * bear_weight_ + hares_needed * hare_weight_;
  if (total != capacity_) return false;
  */


  group = std::move(chosen);
  return true;
}
}
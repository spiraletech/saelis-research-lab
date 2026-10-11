#pragma once
#include <saelis/research.hpp>
#include <saelis/history.hpp>
#include <cstdint>
#include <string>
#include <utility>
namespace saelis {
// All edits through apply() are paired with history records.
// Existing direct Registry writes remain available for compatibility.
class TrackedResearch {
 Registry registry_;
 ResearchHistory history_;
public:
 const Registry& registry() const noexcept { return registry_; }
 const ResearchHistory& history() const noexcept { return history_; }
 bool apply(Candidate candidate, std::int64_t timestamp,
            std::string actor, std::string reason, std::string provenance_url) {
  const auto candidate_assessment=assess(candidate);
  if(!candidate_assessment) return false;
  const Candidate* existing=registry_.find(candidate.id);
  const int prior=existing?assess(*existing)->score:0;
  Registry staged_registry=registry_;
  ResearchHistory staged_history=history_;
  if(existing) {
   if(!staged_registry.replace(std::move(candidate))) return false;
  } else if(!staged_registry.add(std::move(candidate))) return false;
  const auto& entries=staged_history.entries();
  if(!entries.empty() && entries.back().sequence==UINT64_MAX) return false;
  ResearchChange change{entries.empty()?1:entries.back().sequence+1,
   timestamp,candidate_assessment->id,std::move(actor),std::move(reason),
   std::move(provenance_url),prior,candidate_assessment->score};
  if(!staged_history.append(std::move(change))) return false;
  registry_=std::move(staged_registry);
  history_=std::move(staged_history);
  return true;
 }
};
}

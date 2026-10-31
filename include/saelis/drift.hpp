#pragma once
// Advisory, side-effect-free SpiralOS input/output drift and maturity assessment.
// All check values are provided by trusted callers; this module does not inspect
// model semantics, authenticate approvals, or verify hashes against stored bytes.
#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

namespace saelis::governance {

enum class PowerLevel {
  Unclassified, Foundational, Awakened, Enlightened, Ascended,
  ApproachingSuperRare, SuperRare, Transcendent
};
enum class ShadowCore { Clear, Corrupted, Defeated, Soulless };
enum class Check { Unknown, Pass, Fail };
enum class DriftSignal {
  MissingProvenance, UnapprovedInput, UnverifiedEvidence,
  EvidenceCorruption, UnauthorizedMutation, CanonConflict,
  FailedBuildOrTests, Regression, IntentMismatch, IdentityLoss,
  IncompleteReview
};

// The user-authored source and approved intent remain separate. No AI output
// can supersede either value. Digests are opaque SHA-256 hex strings.
struct InputOutputRecord {
  std::string user_input_sha256;
  std::string output_sha256;
  std::string approved_intent_revision;
  std::string canon_revision;
  bool user_approved{false};
  Check evidence_integrity{Check::Unknown};
  Check mutation_authorized{Check::Unknown};
  Check canon_preserved{Check::Unknown};
  Check build_and_tests{Check::Unknown};
  Check regression_safe{Check::Unknown};
  Check instruction_preserved{Check::Unknown};
  Check creative_identity_preserved{Check::Unknown};
};

// Evidence gates are NOT claims until upstream test receipts are authenticated.
struct LevelEvidence {
  bool requirements_approved{false};
  bool authority_boundaries_recorded{false};
  bool acceptance_tests_defined{false};
  bool clean_build{false};
  bool unit_tests_passed{false};
  bool complete_user_journey{false};
  bool deterministic_replay_passed{false};
  bool integration_passed{false};
  bool fault_isolation_passed{false};
  bool benchmark_passed{false};
  bool security_review_passed{false};
  bool independent_review_passed{false};
  bool release_receipt{false};
  bool sustained_release_history{false};
  bool governance_audit_passed{false};
};

struct DriftAssessment {
  PowerLevel evidence_level{PowerLevel::Unclassified};
  ShadowCore shadow{ShadowCore::Clear};
  std::vector<DriftSignal> signals;
  bool promotion_allowed{false};
  bool requires_review{true};
};

inline bool valid_sha256(std::string_view digest) {
  if (digest.size() != 64) return false;
  return std::all_of(digest.begin(), digest.end(), [](char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
  });
}

inline PowerLevel assess_power(const LevelEvidence& e) {
  if (!(e.requirements_approved && e.authority_boundaries_recorded &&
        e.acceptance_tests_defined)) return PowerLevel::Unclassified;
  if (!(e.clean_build && e.unit_tests_passed)) return PowerLevel::Foundational;
  if (!(e.complete_user_journey && e.deterministic_replay_passed)) return PowerLevel::Awakened;
  if (!(e.integration_passed && e.fault_isolation_passed)) return PowerLevel::Enlightened;
  if (!(e.benchmark_passed && e.security_review_passed)) return PowerLevel::Ascended;
  if (!(e.independent_review_passed && e.release_receipt)) return PowerLevel::ApproachingSuperRare;
  if (!(e.sustained_release_history && e.governance_audit_passed)) return PowerLevel::SuperRare;
  return PowerLevel::Transcendent;
}

inline DriftAssessment assess_drift(const InputOutputRecord& r, const LevelEvidence& e) {
  DriftAssessment a;
  a.evidence_level = assess_power(e);
  auto flag = [&](DriftSignal s) { a.signals.push_back(s); };
  if (!valid_sha256(r.user_input_sha256) || !valid_sha256(r.output_sha256) ||
      r.approved_intent_revision.empty() || r.canon_revision.empty())
    flag(DriftSignal::MissingProvenance);
  if (!r.user_approved) flag(DriftSignal::UnapprovedInput);
  if (r.evidence_integrity == Check::Fail) flag(DriftSignal::EvidenceCorruption);
  if (r.mutation_authorized == Check::Fail) flag(DriftSignal::UnauthorizedMutation);
  if (r.canon_preserved == Check::Fail) flag(DriftSignal::CanonConflict);
  if (r.build_and_tests == Check::Fail) flag(DriftSignal::FailedBuildOrTests);
  if (r.regression_safe == Check::Fail) flag(DriftSignal::Regression);
  if (r.instruction_preserved == Check::Fail) flag(DriftSignal::IntentMismatch);
  if (r.creative_identity_preserved == Check::Fail) flag(DriftSignal::IdentityLoss);
  const bool verified =
    r.evidence_integrity == Check::Pass && r.mutation_authorized == Check::Pass &&
    r.canon_preserved == Check::Pass && r.build_and_tests == Check::Pass &&
    r.regression_safe == Check::Pass && r.instruction_preserved == Check::Pass &&
    r.creative_identity_preserved == Check::Pass;
  if (!verified) flag(DriftSignal::UnverifiedEvidence);
  if (r.evidence_integrity == Check::Fail || r.mutation_authorized == Check::Fail ||
      r.canon_preserved == Check::Fail) a.shadow = ShadowCore::Corrupted;
  else if (r.build_and_tests == Check::Fail || r.regression_safe == Check::Fail)
    a.shadow = ShadowCore::Defeated;
  else if (r.instruction_preserved == Check::Fail ||
           r.creative_identity_preserved == Check::Fail)
    a.shadow = ShadowCore::Soulless;
  a.promotion_allowed = a.evidence_level != PowerLevel::Unclassified &&
     a.shadow == ShadowCore::Clear && a.signals.empty() && r.user_approved;
  a.requires_review = !a.promotion_allowed;
  return a;
}

} // namespace saelis::governance

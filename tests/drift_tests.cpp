#include <saelis/drift.hpp>
#include <cassert>
#include <algorithm>
using namespace saelis::governance;

bool has(const DriftAssessment& a, DriftSignal s) {
  return std::find(a.signals.begin(), a.signals.end(), s) != a.signals.end();
}

int main() {
  LevelEvidence e;
  assert(assess_power(e) == PowerLevel::Unclassified);
  e.requirements_approved = e.authority_boundaries_recorded = e.acceptance_tests_defined = true;
  assert(assess_power(e) == PowerLevel::Foundational);
  e.clean_build = e.unit_tests_passed = true;
  assert(assess_power(e) == PowerLevel::Awakened);
  e.complete_user_journey = e.deterministic_replay_passed = true;
  assert(assess_power(e) == PowerLevel::Enlightened);
  e.integration_passed = e.fault_isolation_passed = true;
  assert(assess_power(e) == PowerLevel::Ascended);
  e.benchmark_passed = e.security_review_passed = true;
  assert(assess_power(e) == PowerLevel::ApproachingSuperRare);
  e.independent_review_passed = e.release_receipt = true;
  assert(assess_power(e) == PowerLevel::SuperRare);
  e.sustained_release_history = e.governance_audit_passed = true;
  assert(assess_power(e) == PowerLevel::Transcendent);

  InputOutputRecord io;
  io.user_input_sha256 = std::string(64, 'a');
  io.output_sha256 = std::string(64, 'f');
  io.approved_intent_revision = "user-approved-intent-v1";
  io.canon_revision = "canon-main-1";
  io.user_approved = true;
  io.evidence_integrity = io.mutation_authorized = io.canon_preserved =
    io.build_and_tests = io.regression_safe = io.instruction_preserved =
    io.creative_identity_preserved = Check::Pass;
  auto clean = assess_drift(io, e);
  assert(clean.promotion_allowed);
  assert(!clean.requires_review);
  assert(clean.shadow == ShadowCore::Clear);
  assert(clean.evidence_level == PowerLevel::Transcendent);

  auto unclear = io;
  unclear.instruction_preserved = Check::Unknown;
  auto unknown = assess_drift(unclear, e);
  assert(unknown.shadow == ShadowCore::Clear);
  assert(!unknown.promotion_allowed);
  assert(has(unknown, DriftSignal::UnverifiedEvidence));

  auto intent = io;
  intent.instruction_preserved = Check::Fail;
  auto soulless = assess_drift(intent, e);
  assert(soulless.shadow == ShadowCore::Soulless);
  assert(has(soulless, DriftSignal::IntentMismatch));
  assert(!soulless.promotion_allowed);

  auto failed = io;
  failed.build_and_tests = Check::Fail;
  auto defeated = assess_drift(failed, e);
  assert(defeated.shadow == ShadowCore::Defeated);
  assert(has(defeated, DriftSignal::FailedBuildOrTests));

  auto corrupted = io;
  corrupted.mutation_authorized = Check::Fail;
  corrupted.creative_identity_preserved = Check::Fail;
  assert(assess_drift(corrupted, e).shadow == ShadowCore::Corrupted);

  auto missing = io;
  missing.output_sha256 = "this-is-not-a-sha256";
  assert(!assess_drift(missing, e).promotion_allowed);
  assert(has(assess_drift(missing, e), DriftSignal::MissingProvenance));
  auto not_approved = io;
  not_approved.user_approved = false;
  assert(has(assess_drift(not_approved, e), DriftSignal::UnapprovedInput));
  assert(assess_drift(not_approved, e).shadow == ShadowCore::Clear);
  assert(!valid_sha256(std::string(64, 'G')));
  assert(valid_sha256(std::string(64, '0')));
}

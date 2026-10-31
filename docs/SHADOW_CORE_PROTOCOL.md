# SpiralOS / SAELIS — Input/Output Drift + SHADOW CORE v1 (PROPOSED)

Status: **review branch, not canon promotion or running system service**.
Owner: SAELIS Research Lab (read-only assessment). GOLF-GUFF remains the
execution/permissions authority; HOME remains canonical world-state authority.
No HAKUI, audio, or renderer execution authority is transferred.

## Research lineage and design rationale
Computing moved from reproducible stored programs and typed languages to
interactive GUIs, modifiable game engines, data-driven simulations, and
AI-assisted artifact production. The Sims teaches persistent actor/object
relationships; The Urbz, social consequences; Tony Hawk, predictable responsive
input; Harry Potter games, contextual world interaction; Newgrounds/Flash
microgames, efficient creative publishing; narrative Flash adventures, small
state machines. These are design lessons, NOT licenses to copy code or assets.
For the 2026 engineering era, prioritize constrained autonomy, test receipts,
isolation, component boundaries, local efficiency, and reversible deployment.

## Truth hierarchy: input, intent, output, evidence
1. **USER SOURCE:** The user's original words/requirements are the authoritative
   source of **their intent**, not necessarily of external empirical facts.
   Record exact source reference, digest and approved corrections; never replace
   with an AI paraphrase. Raw personal text requires intentional retention rules.
2. **APPROVED INTENT:** Human-approved interpretation and revision, linked to
   the source. Ambiguity is surfaced, not silently resolved as canonical truth.
3. **CANON BASELINE:** Pinned repository revision and existing authority map.
4. **OUTPUT:** Candidate artifact, commit, test result and content digest.
   Generated text is not proof; a successful build is not proof of correctness.
5. **VERIFICATION:** Actual build/test/benchmark receipts, provenance and
   appropriate human validation. Trustworthiness is not derivable from a hash.
6. **PROMOTION:** GOLF-GUFF authorization gate plus explicit user approval.
   SAELIS scores/flags only. Never auto-merge, auto-promote or self-authorize.

Workflow: MEMORY -> LOGIC -> DRIFT ANALYSIS -> VERIFICATION ->
APPROVED EVOLUTION -> PRESERVED CANON.

For each requested change produce an InputOutputRecord. The caller must link
the actual input/output bytes to cryptographic SHA-256 digests, validate
approval provenance, and classify each Check as Pass, Fail or Unknown.
Unknown never silently means Pass. This library compares structured, attested
signals; it does NOT read minds, parse semantics, authenticate signatures or
independently validate evidence.

## Power levels (original SpiralOS terminology)
- **FOUNDATIONAL** — approved requirements, authority map, acceptance tests.
- **AWAKENED** — clean build and verified unit tests.
- **ENLIGHTENED** — complete user journey and deterministic replay.
- **ASCENDED** — cross-module integration and fault isolation.
- **APPROACHING SUPER RARE** — measured benchmarks and security review.
- **SUPER RARE** — independent product review and release receipt.
- **TRANSCENDENT** — sustained release history and governance audit.

Evidence gates are cumulative. An evidence score is descriptive and never
authorizes modification. Incomplete baseline is UNCLASSIFIED. Existing products
must be evaluated against actual records, never automatically awarded a tier.
No externally borrowed transformation names belong to this taxonomy.

## SHADOW CORE (cross-cutting defect state, not a person's identity)
- **CLEAR** — no recognized shadow defect. Missing evidence still blocks promotion.
- **CORRUPTED** — broken evidence integrity, unauthorized mutation, or canon
  conflict. Action: quarantine change, preserve receipts, restore known baseline
  subject to existing recovery authority.
- **DEFEATED** — build/test failure or confirmed regression. Action: reject
  candidate, triage, retest, and revert if warranted/authorized.
- **SOULLESS** — technically plausible output loses approved user intent or
  project creative identity. Action: pause promotion; return to the user's exact
  source and seek creative review.

A change may contain multiple drift signals. The single displayed Shadow
Core state uses priority CORRUPTED > DEFEATED > SOULLESS; underlying signals
remain available. Do not apply these labels to people or mental health.

## Examples
- Output builds, tests pass, but changes the user's specified 7-band mapping:
  **SOULLESS** (intent mismatch), no promotion.
- Plugin removal crashes its host: **DEFEATED** if confirmed by a test receipt.
- An agent silently changes HOME authority: **CORRUPTED**.
- A generated patch with no test receipt: **CLEAR + UNVERIFIED**, not success.
- A successful release with verified gates: qualify for a measured power tier,
  then await human approval before promoting canon.

## Current implementation boundary
Header: `include/saelis/drift.hpp`; tests: `tests/drift_tests.cpp`.
It is a deterministic, side-effect-free evaluator of caller-supplied
structured checks. Does not access conversations, models, repos, files, or
perform background monitoring. The testing fixture demonstrates classification
logic only; it does not validate the authenticity of input digests or receipts.
To deploy it, first implement an authenticated source/approval/receipt adapter
under existing permissions and run platform CI + independent acceptance tests.

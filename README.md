# SAELIS Research Lab

C++20 deterministic technology-assessment kernel. Records candidates and supporting evidence; scores and ranks candidates. Does not browse, invoke AI models or independently validate evidence.

Build:
```sh
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Formula: score = 4*utility + 2*maturity - 2*integration_cost - 3*risk. Each input is 0..5. Ranking prioritizes candidates with complete evidence entries before scores, then stable ID tie-breaking. Scores are advisory, not automatic deployment decisions.

Integration: SAELIS Research Lab owns research assessment, not HAKUI game execution. `spiral-core-sdk` supplies shared infrastructure; `SPIRAL-OS-HOME` owns canonical world-state; `hakui` owns gameplay. The former `spiralos-ai-genius` repository was renamed to `hakui-main-repos` and is not the SAELIS orchestrator. Persistent checkpoint and history components exist but still require comprehensive cross-platform verification, concurrency protection and durability review.

## Optional Spiral Core SDK integration (B2)

Research assessment transport lives in `include/saelis/integration.hpp`. It serializes a validated `Assessment` through the `Spiral::Core` versioned message codec, using topic `saelis.research.assessment.v1`. This is a transport adapter only: no canonical world-state authority, networking, authentication, or implicit research mutations.

With sibling local GitHub checkouts:
```sh
cmake -S saelis-research-lab -B build-research -DBUILD_TESTING=ON -DSAELIS_BUILD_CORE_INTEGRATION=ON -DSPIRAL_CORE_SDK_DIR=/absolute/path/to/spiral-core-sdk
cmake --build build-research --config Release
ctest --test-dir build-research -C Release --output-on-failure
```

The adapter rejects wrong topics, corrupted or malformed frames, unsupported score ranges and invalid candidate identifiers. The integrity checksum is not authentication. Evidence validity is the library's existing structural check, not a fact-check. The local isolated fixture passed; CI against the full current repositories remains to be verified.

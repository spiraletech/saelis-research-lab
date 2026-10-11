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

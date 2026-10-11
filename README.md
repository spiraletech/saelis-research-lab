# SAELIS Research Lab

C++20 deterministic technology-assessment kernel. Records candidates and supporting evidence; scores and ranks candidates. Does not browse, invoke AI models or independently validate evidence.

Build:
```sh
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Formula: score = 4*utility + 2*maturity - 2*integration_cost - 3*risk. Each input is 0..5. Ranking prioritizes candidates with complete evidence entries before scores, then stable ID tie-breaking. Scores are advisory, not automatic deployment decisions.

Integration: spiralos-ai-genius orchestrates AI tools; SPIRAL-OS-HOME owns canonical world state; Spiral::Core provides common C++ infrastructure. Next steps: persistent format, authenticated provider adapters, real benchmarks and reproducible reports.

#pragma once
#include <optional>
#include <string>
#include <vector>
namespace saelis {
struct Evidence { std::string source_url; std::string summary; };
struct Candidate {
 std::string id;
 std::string title;
 int utility{}; int integration_cost{}; int maturity{}; int risk{};
 std::vector<Evidence> evidence;
};
struct Assessment { std::string id; int score{}; bool evidenced{}; };
std::optional<Assessment> assess(const Candidate& candidate);
class Registry {
 std::vector<Candidate> items_;
 public:
 bool add(Candidate candidate);
 const Candidate* find(const std::string& id) const;
 std::vector<Assessment> rank() const;
};
}

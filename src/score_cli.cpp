#include <saelis/research.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <charconv>
#include <system_error>

namespace {
bool parse_score(std::string_view text, int& out) {
  if(text.empty()) return false;
  const auto* begin=text.data();
  const auto* end=begin+text.size();
  const auto result=std::from_chars(begin,end,out);
  return result.ec==std::errc{} && result.ptr==end && out>=0 && out<=5;
}
void help() {
  std::cerr << "Usage: saelis_score <id> <title> <utility:0..5> <cost:0..5> <maturity:0..5> <risk:0..5> [source_url evidence_summary]\n";
}
}
int main(int argc, char** argv) {
  if(argc!=7 && argc!=9) { help(); return 2; }
  saelis::Candidate candidate;
  candidate.id=argv[1]; candidate.title=argv[2];
  if(!parse_score(argv[3],candidate.utility) ||
     !parse_score(argv[4],candidate.integration_cost) ||
     !parse_score(argv[5],candidate.maturity) ||
     !parse_score(argv[6],candidate.risk)) {help(); return 2;}
  if(argc==9) candidate.evidence.push_back({argv[7],argv[8]});
  const auto result=saelis::assess(candidate);
  if(!result) {std::cerr<<"Invalid candidate\n";return 2;}
  std::cout<<"id="<<result->id<<" score="<<result->score
           <<" evidenced="<<(result->evidenced?"yes":"no")<<"\n";
  return 0;
}

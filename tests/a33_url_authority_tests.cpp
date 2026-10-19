#include <saelis/research.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
 const std::vector<std::string> invalid{
  "https:///missing-host", "https://?query", "https://#fragment",
  "http://:443/path", "https://user:secret@example.org/path",
  "https://example.org/path\nInjected", "https://example.org/white space",
  "https://example.org/\tsegment", "https://example.org/\x7f", 
  "https://example.org\\evil", "http://", "ftp://example.org/"
 };
 const std::vector<std::string> valid{
  "https://example.org/study", "http://localhost:8080/path",
  "https://[::1]/path", "https://example.org/a?x=1#frag",
  "https://example.org/%0A", "https://example.org/caf\xC3\xA9"
 };
 for(const auto& url:invalid) {
  assert(!saelis::valid_evidence({url,"documented source"}));
  saelis::Candidate c{"test","Research",3,2,4,1,{{url,"documented source"}}};
  const auto a=saelis::assess(c);
  assert(a && !a->evidenced); // retain imported-candidate compatibility
 }
 for(const auto& url:valid) {
  assert(saelis::valid_evidence({url,"documented source"}));
  saelis::Candidate c{"test","Research",3,2,4,1,{{url,"documented source"}}};
  const auto a=saelis::assess(c);
  assert(a && a->evidenced);
 }
 std::cout<<"A33: "<<invalid.size()<<" invalid URL cases and "<<valid.size()<<" valid URL cases checked in evidence and assessment\n";
}

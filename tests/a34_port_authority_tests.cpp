#include <saelis/research.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
 const std::vector<std::string> invalid{
  "https://example.org:", "https://example.org:abc", "https://example.org:-1",
  "https://example.org:65536", "https://example.org:999999999999999999999",
  "https://example.org:80:90", "https://example.org:0", "https://example.org:0000", "https://[::1", "https://[]/a",
  "https://[::1]junk", "https://[::1]:", "https://[::1]:xyz",
  "https://[::1]:65536", "https://[::1]:443:80", "https://example.org]",
  "https://[::1]extra:443", "https://[hostname]", "https://[::1]]"
 };
 const std::vector<std::string> valid{
  "https://example.org", "https://example.org:443/path",
  "https://example.org:00080/a", "https://example.org:65535", "http://localhost:8080/path",
  "https://[::1]/path", "https://[::1]:443/a",
  "https://[2001:db8::1]:8443/p?q=1", "https://example.org/caf\xC3\xA9",
  "https://example.org/%0A"
 };
 for(const auto& url:invalid) {
  assert(!saelis::valid_evidence({url,"documented source"}));
  const auto a=saelis::assess({"test","Research",3,2,4,1,{{url,"documented source"}}});
  assert(a && !a->evidenced);
 }
 for(const auto& url:valid) {
  assert(saelis::valid_evidence({url,"documented source"}));
  const auto a=saelis::assess({"test","Research",3,2,4,1,{{url,"documented source"}}});
  assert(a && a->evidenced);
 }
 // Exhaustive decimal-port oracle, including out-of-range values.
 for(unsigned port=0;port<=70000;port++) {
  for(const std::string& host : {std::string("example.org"),std::string("[::1]")}) {
   const auto url=std::string("https://")+host+":"+std::to_string(port)+"/study";
   assert(saelis::valid_evidence({url,"documented source"}) == (port>=1 && port<=65535));
  }
 }
 std::cout << "A34 authority/port cases: " << invalid.size() << " invalid and " << valid.size() << " valid\n";
}

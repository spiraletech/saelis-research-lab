#include <saelis/research.hpp>
#include <cassert>
int main(){
 saelis::Registry r;
 saelis::Candidate strong{"local-inference","Local inference",5,2,4,1,{{"https://example.org/study","Reproducible benchmark"}}};
 assert(r.add(strong)); assert(!r.add(strong));
 assert(r.find("local-inference")!=nullptr); assert(!r.find("missing"));
 saelis::Candidate weak{"concept","Unverified idea",5,0,5,0,{}};
 assert(r.add(weak)); assert(r.rank().front().id=="local-inference");
 auto invalid=weak; invalid.id="negative"; invalid.risk=-1; assert(!r.add(invalid));
 assert(saelis::assess(strong)->score==21); assert(!saelis::assess(invalid));
}

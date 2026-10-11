#include <saelis/tracked_research.hpp>
#include <cassert>
int main(){
 saelis::TrackedResearch tracked;
 saelis::Candidate item{"model","Candidate",4,2,4,1,{{"https://example.org/e","benchmark"}}};
 assert(tracked.apply(item,1700000000,"saelis","initial measurement","https://example.org/e"));
 assert(tracked.registry().find("model"));
 assert(tracked.history().entries().size()==1);
 assert(tracked.history().entries().front().prior_score==0);
 const int first=saelis::assess(*tracked.registry().find("model"))->score;
 item.utility=5;
 assert(!tracked.apply(item,1700000001,"","missing actor",""));
 assert(tracked.history().entries().size()==1);
 assert(saelis::assess(*tracked.registry().find("model"))->score==first);
 assert(!tracked.apply(item,1699999999,"saelis","clock rollback",""));
 assert(!tracked.apply(item,1700000001,"saelis","invalid provenance","file:///bad"));
 assert(tracked.history().entries().size()==1);
 assert(tracked.apply(item,1700000001,"human","verified rebenchmark","https://example.org/e"));
 assert(tracked.history().entries().size()==2);
 assert(tracked.history().entries().back().prior_score==first);
 assert(tracked.history().entries().back().new_score==first+4);
 assert(saelis::assess(*tracked.registry().find("model"))->score==first+4);
}

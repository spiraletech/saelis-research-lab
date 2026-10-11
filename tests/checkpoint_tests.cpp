#include <saelis/checkpoint.hpp>
#include <cassert>
#include <fstream>
int main(){
 saelis::Registry r;saelis::ResearchHistory h;
 const auto path="checkpoint_test.bin";
 assert(r.add({"a","Model A",4,2,3,1,{{"https://example.org/a","Test"}}}));
 const auto score=saelis::assess(*r.find("a"))->score;
 assert(h.append({1,1700000000,"a","tester","initial score","https://example.org/a",0,score}));
 assert(saelis::ResearchCheckpoint::save(r,h,path));
 saelis::Registry restored;saelis::ResearchHistory history;
 assert(saelis::ResearchCheckpoint::load(restored,history,path));
 assert(restored.find("a")&&history.entries().size()==1);
 {std::fstream file(path,std::ios::in|std::ios::out|std::ios::binary);file.seekp(30);file.put('X');}
 assert(!saelis::ResearchCheckpoint::load(restored,history,path));
 assert(restored.find("a")&&history.entries().size()==1);
 assert(!saelis::ResearchCheckpoint::load(restored,history,"missing.bundle"));
}

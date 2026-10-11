#include <saelis/history.hpp>
#include <cassert>
#include <fstream>
int main(){
 saelis::ResearchHistory h;
 assert(h.append({1,1700000000,"model-a","saelis","baseline","https://example.org/report",0,12}));
 assert(!h.append({3,1700000001,"model-a","saelis","skip","",12,13}));
 assert(!h.append({2,1699999999,"model-a","saelis","time reversal","",12,13}));
 assert(!h.append({2,1700000001,"model-a","saelis","invalid provenance","file:///etc/passwd",12,13}));
 assert(h.append({2,1700000001,"model-a","human","benchmark updated","",12,16}));
 assert(h.save("history_test.txt"));
 saelis::ResearchHistory restored;
 assert(restored.load("history_test.txt"));
 assert(restored.entries().size()==2);
 assert(restored.entries()[1].new_score==16);
 {std::ofstream f("history_test.txt");f<<"SAELIS_HISTORY_V1 1\n1 1 \"id\" \"actor\" \"reason\" \"\" 1 2\nEXTRA";}
 assert(!restored.load("history_test.txt"));
 assert(restored.entries().size()==2);
 {std::ofstream f("history_test.txt");f<<"SAELIS_HISTORY_V2 0\n";}
 assert(!restored.load("history_test.txt"));
}

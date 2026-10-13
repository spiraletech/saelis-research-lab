#include <saelis/history.hpp>
#include <cassert>
#include <fstream>

int main() {
 using saelis::ResearchHistory;
 ResearchHistory history;
 // Reject a fabricated nonzero baseline without changing state.
 assert(!history.append({1,1700000000,"a","human","bad baseline","",5,10}));
 assert(history.entries().empty());
 assert(history.append({1,1700000000,"a","human","baseline","",0,-5}));
 // The rule applies independently to candidates introduced later.
 assert(!history.append({2,1700000001,"b","human","bad first score","",-5,12}));
 assert(history.entries().size()==1);
 assert(history.append({2,1700000001,"b","human","new candidate","",0,12}));
 assert(!history.append({3,1700000002,"a","human","broken chain","",0,9}));
 assert(history.append({3,1700000002,"a","human","valid change","",-5,9}));
 assert(history.save("a11_history_valid.txt"));
 ResearchHistory restored;
 assert(restored.load("a11_history_valid.txt"));
 assert(restored.entries().size()==3);
 // Valid V1 syntax does not make an invalid first prior_score legitimate.
 { std::ofstream out("a11_history_invalid.txt",std::ios::binary);
   out << "SAELIS_HISTORY_V1 1\n1 1700000000 \"x\" \"human\" \"initial\" \"\" 7 12\n"; }
 assert(!restored.load("a11_history_invalid.txt"));
 assert(restored.entries().size()==3); // Failed import must be transactional.
}

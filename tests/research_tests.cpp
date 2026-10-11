#include <saelis/research.hpp>
#include <cassert>
#include <fstream>
int main(){
 saelis::Registry r;
 saelis::Candidate strong{"local-inference","Local inference",5,2,4,1,{{"https://example.org/study","Reproducible benchmark"}}};
 assert(r.add(strong));assert(!r.add(strong));
 assert(r.find("local-inference"));assert(!r.find("missing"));
 saelis::Candidate weak{"concept","Unverified idea",5,0,5,0,{}};
 assert(r.add(weak));assert(r.rank().front().id=="local-inference");
 auto invalid=weak;invalid.id="negative";invalid.risk=-1;assert(!r.add(invalid));
 assert(saelis::assess(strong)->score==21);assert(!saelis::assess(invalid));
 assert(saelis::valid_evidence(strong.evidence[0]));
 assert(!saelis::valid_evidence({"file:///secret","unsupported"}));
 auto badEvidence=strong;badEvidence.evidence={{"file:///secret","unsupported"}};badEvidence.id="unverified";
 assert(r.add(badEvidence));assert(!saelis::assess(badEvidence)->evidenced);
 const auto path="saelis_research_roundtrip.txt";
 assert(r.save(path));saelis::Registry loaded;assert(loaded.load(path));
 assert(loaded.find("local-inference"));assert(loaded.find("unverified"));
 assert(loaded.rank().size()==3);assert(loaded.rank().front().id=="local-inference");
 {std::ofstream out(path,std::ios::trunc);out<<"SAELIS_RESEARCH_V1 1\n\"invalid\" \"Broken\" 8 0 0 0 0\n";}
 assert(!loaded.load(path));assert(loaded.find("local-inference"));
 {std::ofstream out(path,std::ios::trunc);out<<"SAELIS_RESEARCH_V2 0\n";}
 assert(!loaded.load(path));
 {std::ofstream out(path,std::ios::trunc);out<<"SAELIS_RESEARCH_V1 0\nEXTRA\n";}
 assert(!loaded.load(path));
}

#pragma once
#include <saelis/research.hpp>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <limits>
#include <optional>
#include <string>
#include <utility>
#include <vector>
namespace saelis {
struct ResearchChange {
 std::uint64_t sequence{};
 std::int64_t unix_seconds{};
 std::string candidate_id;
 std::string actor;
 std::string reason;
 std::string evidence_url;
 int prior_score{};
 int new_score{};
};
class ResearchHistory {
 std::vector<ResearchChange> records_;
 static bool valid(const ResearchChange& e) {
  return e.unix_seconds>=0 && !e.candidate_id.empty() && !e.actor.empty() &&
    !e.reason.empty() && e.candidate_id.size()<=256 && e.actor.size()<=256 &&
    e.reason.size()<=4096 && e.evidence_url.size()<=2048 &&
    (e.evidence_url.empty() || valid_evidence({e.evidence_url,"provenance reference"}));
 }
public:
 const std::vector<ResearchChange>& entries() const noexcept {return records_;}
 bool append(ResearchChange e) {
  if(records_.size()>=100000 || !valid(e))return false;
  const auto expected=records_.empty()?1:records_.back().sequence+1;
  if(!expected || e.sequence!=expected || (!records_.empty() && e.unix_seconds<records_.back().unix_seconds))return false;
  records_.push_back(std::move(e));return true;
 }
 bool save(const std::string& path) const {
  std::ofstream out(path,std::ios::binary|std::ios::trunc);
  if(!out)return false;
  out<<"SAELIS_HISTORY_V1 "<<records_.size()<<'\n';
  for(const auto& e:records_)
   out<<e.sequence<<' '<<e.unix_seconds<<' '<<std::quoted(e.candidate_id)<<' '
      <<std::quoted(e.actor)<<' '<<std::quoted(e.reason)<<' '
      <<std::quoted(e.evidence_url)<<' '<<e.prior_score<<' '<<e.new_score<<'\n';
  out.flush();return out.good();
 }
 bool load(const std::string& path) {
  std::ifstream in(path,std::ios::binary);
  std::string version;std::size_t count{};
  if(!(in>>version>>count)||version!="SAELIS_HISTORY_V1"||count>100000)return false;
  ResearchHistory staged;
  for(std::size_t i=0;i<count;++i){
   ResearchChange e;
   if(!(in>>e.sequence>>e.unix_seconds>>std::quoted(e.candidate_id)
        >>std::quoted(e.actor)>>std::quoted(e.reason)
        >>std::quoted(e.evidence_url)>>e.prior_score>>e.new_score)
        ||!staged.append(std::move(e)))return false;
  }
  in>>std::ws;if(!in.eof())return false;
  records_.swap(staged.records_);return true;
 }
};
}

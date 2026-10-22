#include <saelis/research.hpp>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string_view>
#include <utility>
namespace saelis {
namespace {
constexpr std::size_t max_records=100000, max_evidence=1000;
bool valid_score(int x){return x>=0&&x<=5;}
bool valid_text(const std::string& text){return !text.empty() && text.size()<=8192;}
// Conservative authority check: do not treat malformed host:port syntax as evidence.
// This is structural validation, not DNS lookup or complete URL/IPv6 validation.
bool valid_authority(std::string_view authority){
 if(authority.empty() || authority.find('@')!=std::string_view::npos) return false;
 std::string_view host, port;
 if(authority.front()=='['){
  const auto close=authority.find(']');
  if(close==std::string_view::npos || close<=1) return false;
  host=authority.substr(1,close-1);
  if(host.find(':')==std::string_view::npos) return false;
  const auto suffix=authority.substr(close+1);
  if(!suffix.empty()){
   if(suffix.front()!=':') return false;
   port=suffix.substr(1);
   if(port.empty()) return false;
  }
 }else{
  if(authority.find_first_of("[]")!=std::string_view::npos) return false;
  const auto colon=authority.find(':');
  host=authority.substr(0,colon);
  if(colon!=std::string_view::npos){
   port=authority.substr(colon+1);
   if(port.empty()) return false;
  }
 }
 if(host.empty()) return false;
 if(!port.empty()){
  unsigned value=0;
  for(const char digit:port){
   if(digit<'0' || digit>'9') return false;
   const unsigned next=static_cast<unsigned>(digit-'0');
   if(value>(65535u-next)/10u) return false;
   value=value*10u+next;
  }
  if(value==0) return false;
 }
 return true;
}
}
bool valid_evidence(const Evidence& e){
 const bool https=e.source_url.starts_with("https://");
 const bool http=e.source_url.starts_with("http://");
 const auto offset=https?8u:7u;
 if(!(https||http) || e.source_url.size()>2048 || !valid_text(e.summary)) return false;
 const std::string_view url=e.source_url;
 const auto end=url.find_first_of("/?#",offset);
 const auto authority=url.substr(offset,end==std::string_view::npos?end:end-offset);
 // A URL without a host or with embedded credentials is not reliable provenance.
 if(!valid_authority(authority)) return false;
 // Reject raw whitespace, control bytes, DEL and browser-normalized backslashes.
 for(const unsigned char byte:e.source_url)
  if(byte<=0x20 || byte==0x7f || byte=='\\') return false;
 return true;
}
std::optional<Assessment> assess(const Candidate& c){
 if(!valid_text(c.id)||!valid_text(c.title)||!valid_score(c.utility)||!valid_score(c.integration_cost)||!valid_score(c.maturity)||!valid_score(c.risk)||c.evidence.size()>max_evidence) return std::nullopt;
 bool evidenced=!c.evidence.empty();
 for(const auto& e:c.evidence) if(!valid_evidence(e)) evidenced=false;
 return Assessment{c.id,4*c.utility+2*c.maturity-2*c.integration_cost-3*c.risk,evidenced};
}
bool Registry::add(Candidate c){
 if(!assess(c)||find(c.id)!=nullptr||items_.size()>=max_records) return false;
 items_.push_back(std::move(c));return true;
}
bool Registry::replace(Candidate candidate){
 if(!assess(candidate)) return false;
 auto it=std::find_if(items_.begin(),items_.end(),[&](const auto& c){return c.id==candidate.id;});
 if(it==items_.end()) return false;
 auto staged=items_;
 staged[static_cast<std::size_t>(it-items_.begin())]=std::move(candidate);
 items_.swap(staged);
 return true;
}
const Candidate* Registry::find(const std::string& id) const {
 const auto it=std::find_if(items_.begin(),items_.end(),[&](const auto& c){return c.id==id;});
 return it==items_.end()?nullptr:&*it;
}
std::vector<Assessment> Registry::rank() const {
 std::vector<Assessment> v;for(const auto& c:items_)v.push_back(*assess(c));
 std::sort(v.begin(),v.end(),[](const auto& a,const auto& b){if(a.evidenced!=b.evidenced)return a.evidenced>b.evidenced;if(a.score!=b.score)return a.score>b.score;return a.id<b.id;});return v;
}
bool Registry::save(const std::string& path) const {
 std::ofstream file(path,std::ios::binary|std::ios::trunc);
 if(!file)return false;
 file<<"SAELIS_RESEARCH_V1 "<<items_.size()<<'\n';
 for(const auto& c:items_){
  file<<std::quoted(c.id)<<' '<<std::quoted(c.title)<<' '<<c.utility<<' '<<c.integration_cost<<' '<<c.maturity<<' '<<c.risk<<' '<<c.evidence.size()<<'\n';
  for(const auto& e:c.evidence)file<<std::quoted(e.source_url)<<' '<<std::quoted(e.summary)<<'\n';
 }
 file.flush();return file.good();
}
bool Registry::load(const std::string& path){
 std::ifstream file(path,std::ios::binary);
 std::string version;std::size_t count;
 if(!(file>>version>>count)||version!="SAELIS_RESEARCH_V1"||count>max_records)return false;
 Registry staged;
 for(std::size_t i=0;i<count;++i){
  Candidate c;std::size_t n;
  if(!(file>>std::quoted(c.id)>>std::quoted(c.title)>>c.utility>>c.integration_cost>>c.maturity>>c.risk>>n)||n>max_evidence)return false;
  for(std::size_t j=0;j<n;++j){
   Evidence e;
   if(!(file>>std::quoted(e.source_url)>>std::quoted(e.summary)))return false;
   c.evidence.push_back(std::move(e));
  }
  if(!staged.add(std::move(c)))return false;
 }
 file>>std::ws;
 if(!file.eof())return false;
 items_.swap(staged.items_);return true;
}
}

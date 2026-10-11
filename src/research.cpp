#include <saelis/research.hpp>
#include <algorithm>
#include <utility>
namespace saelis {
std::optional<Assessment> assess(const Candidate& c){
 auto valid=[](int x){return x>=0&&x<=5;};
 if(c.id.empty()||c.title.empty()||!valid(c.utility)||!valid(c.integration_cost)||!valid(c.maturity)||!valid(c.risk)) return std::nullopt;
 bool evidenced=!c.evidence.empty();
 for(const auto& e:c.evidence) if(e.source_url.empty()||e.summary.empty()) evidenced=false;
 return Assessment{c.id,4*c.utility+2*c.maturity-2*c.integration_cost-3*c.risk,evidenced};
}
bool Registry::add(Candidate c){
 if(!assess(c)||find(c.id)!=nullptr) return false;
 items_.push_back(std::move(c)); return true;
}
const Candidate* Registry::find(const std::string& id) const {
 auto it=std::find_if(items_.begin(),items_.end(),[&](const auto& c){return c.id==id;});
 return it==items_.end()?nullptr:&*it;
}
std::vector<Assessment> Registry::rank() const {
 std::vector<Assessment> v; for(const auto& c:items_) v.push_back(*assess(c));
 std::sort(v.begin(),v.end(),[](const auto& a,const auto& b){if(a.evidenced!=b.evidenced)return a.evidenced>b.evidenced; if(a.score!=b.score)return a.score>b.score;return a.id<b.id;}); return v;
}
}

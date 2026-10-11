#include <saelis/integration.hpp>
#include <cassert>
int main(){
 using namespace saelis;
 Candidate c{"local-inference","Inference",5,2,4,1,{{"https://example.org/study","Benchmark"}}};
 auto frame=integration::assess_and_encode(c);assert(frame);
 auto restored=integration::decode(*frame);assert(restored);
 assert(restored->id==c.id && restored->score==21 && restored->evidenced);
 assert(!integration::encode({"",10,true}));
 assert(!integration::encode({"model",31,false}));
 assert(!integration::decode(std::vector<std::uint8_t>{1,2,3}));
 auto corrupted=*frame;corrupted.back()^=1;assert(!integration::decode(corrupted));
 auto other=spiral::core::MessageCodec::encode({1,"other.topic",{}});assert(other);assert(!integration::decode(*other));
 auto invalid=spiral::core::MessageCodec::encode({1,integration::assessment_topic,{1,0,'a',0,0,2}});assert(invalid);assert(!integration::decode(*invalid));
 auto truncated_payload=spiral::core::MessageCodec::encode({1,integration::assessment_topic,{1,0,'a'}});assert(truncated_payload);assert(!integration::decode(*truncated_payload));
}

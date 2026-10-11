#pragma once
#include <saelis/research.hpp>
#include <spiral/core/contract.hpp>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
namespace saelis::integration {
inline constexpr const char* assessment_topic = "saelis.research.assessment.v1";
inline bool id_valid(const std::string& id){
 if(id.empty() || id.size()>256)return false;
 for(unsigned char ch:id)if(ch<33||ch>126)return false;
 return true;
}
inline std::optional<std::vector<std::uint8_t>> encode(const Assessment& a){
 if(!id_valid(a.id)||a.score < -25 || a.score>30)return std::nullopt;
 std::vector<std::uint8_t> bytes;
 bytes.reserve(a.id.size()+5);
 const std::uint16_t len=static_cast<std::uint16_t>(a.id.size());
 bytes.push_back(static_cast<std::uint8_t>(len));bytes.push_back(static_cast<std::uint8_t>(len>>8));
 for(unsigned char c:a.id)bytes.push_back(c);
 const auto value=static_cast<std::uint16_t>(static_cast<std::int16_t>(a.score));
 bytes.push_back(static_cast<std::uint8_t>(value));bytes.push_back(static_cast<std::uint8_t>(value>>8));
 bytes.push_back(a.evidenced?1:0);
 return spiral::core::MessageCodec::encode({1,assessment_topic,std::move(bytes)});
}
inline std::optional<Assessment> decode(std::span<const std::uint8_t> frame){
 auto result=spiral::core::MessageCodec::decode(frame);
 if(!result||result.message->topic!=assessment_topic)return std::nullopt;
 const auto& p=result.message->payload;
 if(p.size()<5)return std::nullopt;
 const auto n=std::uint16_t(p[0])|(std::uint16_t(p[1])<<8);
 if(n>256||p.size()!=static_cast<std::size_t>(n)+5)return std::nullopt;
 std::string id(p.begin()+2,p.begin()+2+n);
 if(!id_valid(id))return std::nullopt;
 const auto raw=std::uint16_t(p[2+n])|(std::uint16_t(p[3+n])<<8);
 const int score=(raw<=32767)?static_cast<int>(raw):static_cast<int>(raw)-65536;
 const auto evidenced=p[4+n];
 if(score< -25 || score>30 || evidenced>1)return std::nullopt;
 return Assessment{std::move(id),score,evidenced==1};
}
inline std::optional<std::vector<std::uint8_t>> assess_and_encode(const Candidate& c){
 auto a=saelis::assess(c);if(!a)return std::nullopt;return encode(*a);
}
}

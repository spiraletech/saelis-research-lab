#pragma once
#include <saelis/bundle.hpp>
#include <saelis/research.hpp>
#include <saelis/history.hpp>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <system_error>
#include <utility>
namespace saelis {
// Adapter for existing V1 registry and history codecs.
// Single-writer only: scratch paths cannot be shared by concurrent operations.
struct ResearchCheckpoint {
 static bool save(const Registry& registry,const ResearchHistory& history,const std::filesystem::path& path){
  auto a=path;a+=".registry.scratch";auto b=path;b+=".history.scratch";
  const auto cleanup=[&]{std::error_code ec;std::filesystem::remove(a,ec);std::filesystem::remove(b,ec);};
  cleanup();
  if(!registry.save(a.string())||!history.save(b.string())){cleanup();return false;}
  std::ifstream ar(a,std::ios::binary),br(b,std::ios::binary);
  if(!ar||!br){cleanup();return false;}
  std::string raw_a((std::istreambuf_iterator<char>(ar)),{});
  std::string raw_b((std::istreambuf_iterator<char>(br)),{});
  if(ar.bad()||br.bad()){cleanup();return false;}
  const bool result=Bundle::save(path,raw_a,raw_b);
  cleanup();return result;
 }
 static bool load(Registry& registry,ResearchHistory& history,const std::filesystem::path& path){
  std::string raw_a,raw_b;
  if(!Bundle::load(path,raw_a,raw_b))return false;
  auto a=path;a+=".registry.scratch";auto b=path;b+=".history.scratch";
  const auto cleanup=[&]{std::error_code ec;std::filesystem::remove(a,ec);std::filesystem::remove(b,ec);};
  cleanup();
  {std::ofstream ar(a,std::ios::binary|std::ios::trunc),br(b,std::ios::binary|std::ios::trunc);
   if(!ar||!br){cleanup();return false;}
   ar.write(raw_a.data(),static_cast<std::streamsize>(raw_a.size()));
   br.write(raw_b.data(),static_cast<std::streamsize>(raw_b.size()));
   ar.flush();br.flush();
   if(!ar.good()||!br.good()){cleanup();return false;}
  }
  Registry next_registry;ResearchHistory next_history;
  const bool ok=next_registry.load(a.string())&&next_history.load(b.string());
  cleanup();
  if(!ok)return false;
  for(const auto& entry:next_history.entries()){
   // The history may include previous versions of candidates, but every
   // referenced candidate must exist in the present registry.
   if(!next_registry.find(entry.candidate_id))return false;
  }
  for(const auto& entry:next_history.entries()){
   const auto* current=next_registry.find(entry.candidate_id);
   const auto& all=next_history.entries();
   bool is_last=true;
   for(const auto& later:all)if(later.sequence>entry.sequence&&later.candidate_id==entry.candidate_id){is_last=false;break;}
   if(is_last && assess(*current)->score!=entry.new_score)return false;
  }
  registry=std::move(next_registry);history=std::move(next_history);return true;
 }
};
}

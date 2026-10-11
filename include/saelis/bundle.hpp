#pragma once
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
namespace saelis {
// Single-file snapshot envelope: process-crash atomic rename where supported.
// Callers must enforce single-writer access. No power-loss fsync guarantee.
class Bundle {
 static constexpr std::string_view magic="SAELIS_BUNDLE_V1\n";
 static constexpr std::uint64_t max_part=64ULL*1024*1024;
 static std::uint64_t hash(std::string_view data){
  std::uint64_t v=14695981039346656037ULL;
  for(unsigned char c:data){v^=c;v*=1099511628211ULL;}return v;
 }
 static void put64(std::string& s,std::uint64_t n){for(int i=0;i<8;++i)s.push_back(char((n>>(8*i))&255));}
 static bool get64(std::string_view s,std::size_t& pos,std::uint64_t& n){
  if(s.size()-pos<8)return false;
  n=0;for(int i=0;i<8;++i)n|=std::uint64_t(static_cast<unsigned char>(s[pos++]))<<(i*8);
  return true;
 }
public:
 static bool save(const std::filesystem::path& target,std::string_view registry,std::string_view history){
  if(registry.size()>max_part||history.size()>max_part)return false;
  std::string data(magic);put64(data,registry.size());put64(data,history.size());
  data.append(registry);data.append(history);put64(data,hash(data));
  // Refuse symlinks at the destination or staging path to avoid redirection.\n  std::error_code status_ec;\n  if(std::filesystem::is_symlink(std::filesystem::symlink_status(target,status_ec))) return false;\n  auto temp=target;temp+=".pending";\n  status_ec.clear();\n  if(std::filesystem::is_symlink(std::filesystem::symlink_status(temp,status_ec))) return false;
  {std::ofstream out(temp,std::ios::binary|std::ios::trunc);if(!out)return false;
   out.write(data.data(),static_cast<std::streamsize>(data.size()));out.flush();
   if(!out.good()){out.close();std::error_code ec;std::filesystem::remove(temp,ec);return false;}}
  std::error_code ec;std::filesystem::rename(temp,target,ec);
  // Do not remove the old checkpoint to force replacement on Windows.\n  // A failed replacement preserves the old destination and reports failure.\n  if(ec){std::error_code cleanup_ec;std::filesystem::remove(temp,cleanup_ec);return false;}return true;
 }
 static bool load(const std::filesystem::path& target,std::string& registry,std::string& history){
  std::error_code ec;const auto len=std::filesystem::file_size(target,ec);
  if(ec||len>2*max_part+magic.size()+24||len<magic.size()+24)return false;\n  std::error_code status_ec;\n  if(std::filesystem::is_symlink(std::filesystem::symlink_status(target,status_ec)))return false;
  std::ifstream in(target,std::ios::binary);if(!in)return false;
  std::string data(static_cast<std::size_t>(len),'\0');
  in.read(data.data(),static_cast<std::streamsize>(data.size()));if(!in)return false;
  std::string_view s(data);if(!s.starts_with(magic))return false;
  std::size_t pos=magic.size();std::uint64_t a=0,b=0;
  if(!get64(s,pos,a)||!get64(s,pos,b)||a>max_part||b>max_part)return false;
  if(a+b+8!=s.size()-pos)return false;
  const auto checksum_offset=pos+static_cast<std::size_t>(a+b);
  std::size_t check_pos=checksum_offset;std::uint64_t expected=0;
  if(!get64(s,check_pos,expected)||expected!=hash(s.substr(0,checksum_offset)))return false;
  std::string new_registry(s.substr(pos,static_cast<std::size_t>(a)));
  std::string new_history(s.substr(pos+static_cast<std::size_t>(a),static_cast<std::size_t>(b)));
  registry.swap(new_registry);history.swap(new_history);return true;
 }
};
}

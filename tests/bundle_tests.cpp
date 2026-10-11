#include <saelis/bundle.hpp>
#include <cassert>
#include <fstream>
#include <filesystem>
int main(){
 namespace fs=std::filesystem;const fs::path path="test_bundle.bin";
 std::string a="old",b="old";
 assert(saelis::Bundle::save(path,"registry-1","history-1"));
 assert(saelis::Bundle::load(path,a,b));assert(a=="registry-1"&&b=="history-1");
 assert(saelis::Bundle::save(path,"registry-2","history-2"));
 assert(saelis::Bundle::load(path,a,b));assert(a=="registry-2"&&b=="history-2");
 {std::ofstream out(path.string()+".pending",std::ios::binary);out<<"interrupted";}
 assert(saelis::Bundle::load(path,a,b));assert(a=="registry-2");
 {std::fstream file(path,std::ios::in|std::ios::out|std::ios::binary);file.seekp(25);file.put('X');}
 assert(!saelis::Bundle::load(path,a,b));assert(a=="registry-2"&&b=="history-2");
 {std::ofstream file(path,std::ios::trunc);file<<"garbage";}
 assert(!saelis::Bundle::load(path,a,b));
 fs::remove(path);fs::remove(path.string()+".pending");
}

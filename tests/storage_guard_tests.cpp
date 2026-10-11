#include <saelis/bundle.hpp>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>
int main(){
 namespace fs=std::filesystem;
 const auto dir=fs::temp_directory_path()/"saelis_a5_guard_tests";
 fs::create_directories(dir);
 const auto target=dir/"state.bundle";
 assert(saelis::Bundle::save(target,"registry","history"));
 std::string a,b;
 assert(saelis::Bundle::load(target,a,b)&&a=="registry"&&b=="history");
 // Symlink attacks must never overwrite a file outside the checkpoint location.
 const auto outside=dir/"outside.txt";
 {std::ofstream f(outside);f<<"sentinel";}
 std::error_code ec;
 fs::remove(target,ec);
 fs::create_symlink(outside,target,ec);
 if(!ec){
   assert(!saelis::Bundle::save(target,"new","new"));
   assert(!saelis::Bundle::load(target,a,b));
   std::ifstream f(outside);std::string contents;f>>contents;assert(contents=="sentinel");
   fs::remove(target,ec);
 }
 // An abandoned staging symlink is also rejected.
 auto pending=target;pending+=".pending";
 fs::create_symlink(outside,pending,ec);
 if(!ec){
   assert(!saelis::Bundle::save(target,"new","new"));
   fs::remove(pending,ec);
 }
 assert(saelis::Bundle::save(target,"final","history"));
 assert(saelis::Bundle::load(target,a,b)&&a=="final");
 fs::remove_all(dir,ec);
}

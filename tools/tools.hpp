#pragma once
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>

namespace net {
using namespace nlohmann;
json GetStatement(const std::string& problem);
json PostSolution(const std::string& problem, const json& j, bool playground = false);
void DowloadFile(const std::string& url, const std::string filename);
}  // namespace net
namespace base64 {
std::vector<uint8_t> Decode(const std::string&);
std::string Encode(const std::string&);
}
#pragma once
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>

namespace tools {
using namespace nlohmann;
json GetStatement(const std::string& problem);
json PostSolution(const std::string& problem, const json& j);
void DowloadFile(const std::string& url, const std::string filename);
}  // namespace tools
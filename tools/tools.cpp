#include "tools.hpp"

namespace tools {
const std::string access_token = "***REMOVED***";

using namespace nlohmann;
 json GetStatement(const std::string& problem) {
    std::string url =
        "https://hackattic.com/challenges/" + problem + "/problem?access_token=" + access_token;
    return json::parse(cpr::Get(cpr::Url{url}).text);
}
json PostSolution(const std::string& problem, const json& j) {
    std::string url =
        "https://hackattic.com/challenges/" + problem + "/solve?access_token=" + access_token;
    return json::parse(cpr::Post(cpr::Url{url}, cpr::Body{j.dump()}).text);
}
void DowloadFile(const std::string& url, const std::string filename) {
    std::ofstream file(filename);
    auto response = cpr::Download(file, cpr::Url{url});
    if (response.status_code != 200) {
        throw std::runtime_error("Couldn't download file");
    }
}
}  // namespace tools

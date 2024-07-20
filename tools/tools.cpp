#include "tools.hpp"
#include <iostream>
namespace tools {
const std::string access_token = "***REMOVED***";

using namespace nlohmann;
json GetStatement(const std::string& problem) {
    std::string url =
        "https://hackattic.com/challenges/" + problem + "/problem?access_token=" + access_token;
    std::cout << "Getting statement for " << problem << " problem...\n";
    auto resp = json::parse(cpr::Get(cpr::Url{url}).text);
    std::cout << resp.dump(4) << '\n';
    return resp;
}
json PostSolution(const std::string& problem, const json& j, bool playground) {
    std::string url =
        "https://hackattic.com/challenges/" + problem + "/solve?access_token=" + access_token;
    if (playground) {
        url += "&playground=1";
    }
    std::cout << "Posting solution for " << problem << " problem...\n";
    auto resp = json::parse(cpr::Post(cpr::Url{url}, cpr::Body{j.dump()}).text);
    std::cout << "Response: \n" << resp.dump(4) << '\n';
    return resp;
}
void DowloadFile(const std::string& url, const std::string filename) {
    std::cout << "Downloading file from " << url << " into " << filename << "...\n";
    std::ofstream file(filename);
    auto response = cpr::Download(file, cpr::Url{url});
    if (response.status_code != 200) {
        std::cout << "Error!";
    } else {
        std::cout << "Success!";
    }
}
}  // namespace tools

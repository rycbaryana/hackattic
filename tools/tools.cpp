#include "tools.hpp"
#include <iostream>
namespace net {
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
}  // namespace net

namespace base64 {
std::vector<uint8_t> Decode(const std::string& encoded) {
    std::vector<uint8_t> decoded;
    int padding = 0;
    for (size_t i = 0; i < encoded.size(); i += 4) {
        uint32_t binary = 0;
        for (size_t j = 0; j < 4; ++j) {
            char c = encoded[i + j];
            if (c != '=') {
                binary <<= 6;
            } else {
                ++padding;
                binary >>= 2;
            }
            if (c >= 'A' && c <= 'Z')
                binary += c - 'A';
            if (c >= 'a' && c <= 'z')
                binary += c - 'a' + 26;  // skip A-Z
            if (c >= '0' && c <= '9')
                binary += c - '0' + 52;  // skip A-z
            if (c == '+')
                binary += 62;
            if (c == '/')
                binary += 63;
        }
        for (int j = 2 - padding; j >= 0; --j) {
            uint32_t mask = ((1 << 8) - 1) << (j * 8);
            decoded.push_back((binary & mask) >> (j * 8));
        }
    }
    return decoded;
}
std::string Encode(const std::string &data) {
    static constexpr char sEncodingTable[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

    size_t in_len = data.size();
    size_t out_len = 4 * ((in_len + 2) / 3);
    std::string ret(out_len, '\0');
    size_t i;
    char *p = const_cast<char *>(ret.c_str());

    for (i = 0; in_len > 2 && i < in_len - 2; i += 3) {
        *p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
        *p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int)(data[i + 1] & 0xF0) >> 4)];
        *p++ = sEncodingTable[((data[i + 1] & 0xF) << 2) | ((int)(data[i + 2] & 0xC0) >> 6)];
        *p++ = sEncodingTable[data[i + 2] & 0x3F];
    }
    if (i < in_len) {
        *p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
        if (i == (in_len - 1)) {
            *p++ = sEncodingTable[((data[i] & 0x3) << 4)];
            *p++ = '=';
        } else {
            *p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int)(data[i + 1] & 0xF0) >> 4)];
            *p++ = sEncodingTable[((data[i + 1] & 0xF) << 2)];
        }
        *p++ = '=';
    }

    return ret;
}
}
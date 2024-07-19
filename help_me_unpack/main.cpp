#include <bits/stdc++.h>
#include <tools.hpp>

using namespace nlohmann;

using byte = uint8_t;

std::vector<byte> decode(std::string encoded) {
    std::vector<byte> decoded;
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

template <typename T>
T Get(std::vector<byte> bytes, bool big_endian = false) {  // bytes in little_endian
    T x;
    if (big_endian) {
        std::copy(bytes.rbegin(), bytes.rend(), reinterpret_cast<byte*>(&x));
    } else {
        std::copy(bytes.begin(), bytes.end(), reinterpret_cast<byte*>(&x));
    }
    return x;
}

int main() {
    json statement = tools::GetStatement("help_me_unpack");
    std::string encoded = statement["bytes"];
    auto bytes = decode(encoded);
    json solution;
    solution["int"] = Get<int>({bytes.begin(), bytes.begin() + 4});
    solution["uint"] = Get<uint>({bytes.begin() + 4, bytes.begin() + 8});
    solution["short"] = Get<short>({bytes.begin() + 8, bytes.begin() + 10});
    solution["float"] = Get<float>({bytes.begin() + 12, bytes.begin() + 16});
    solution["double"] = Get<double>({bytes.begin() + 16, bytes.begin() + 24});
    solution["big_endian_double"] = Get<double>({bytes.begin() + 24, bytes.begin() + 32}, true);
    tools::PostSolution("help_me_unpack", solution);
    return 0;
}

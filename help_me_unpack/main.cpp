#include <bits/stdc++.h>
#include <tools.hpp>

using namespace nlohmann;

template <typename T>
T Get(std::vector<uint8_t> bytes, bool big_endian = false) {  // bytes in little_endian
    T x;
    if (big_endian) {
        std::copy(bytes.rbegin(), bytes.rend(), reinterpret_cast<uint8_t*>(&x));
    } else {
        std::copy(bytes.begin(), bytes.end(), reinterpret_cast<uint8_t*>(&x));
    }
    return x;
}

int main() {
    json statement = net::GetStatement("help_me_unpack");
    std::string encoded = statement["bytes"];
    auto bytes = base64::Decode(encoded);
    json solution;
    solution["int"] = Get<int>({bytes.begin(), bytes.begin() + 4});
    solution["uint"] = Get<uint>({bytes.begin() + 4, bytes.begin() + 8});
    solution["short"] = Get<short>({bytes.begin() + 8, bytes.begin() + 10});
    solution["float"] = Get<float>({bytes.begin() + 12, bytes.begin() + 16});
    solution["double"] = Get<double>({bytes.begin() + 16, bytes.begin() + 24});
    solution["big_endian_double"] = Get<double>({bytes.begin() + 24, bytes.begin() + 32}, true);
    net::PostSolution("help_me_unpack", solution);
    return 0;
}

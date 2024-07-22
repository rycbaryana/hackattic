#include <bit>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <tools.hpp>

std::string sha256(const std::string &str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}
int sha256_difficulty(const std::string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    int difficulty = 0;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        int count = std::countl_zero(hash[i]);
        difficulty += count;
        if (count != 8) {
            break;
        }
    }
    return difficulty;
}

using namespace nlohmann;

int find_nonce(json data, int difficulty) {
    int i = 0;
    data["nonce"] = i;
    while (sha256_difficulty(data.dump()) < difficulty) {
        i++;
        data["nonce"] = i;
    }
    return i;
}

const std::string problem = "mini_miner";

int main() {
    json j = net::GetStatement(problem);

    int difficulty = j["difficulty"];
    json block = j["block"];
    std::cout << "Finding difficulty " << difficulty << " nonce for " << block << '\n';
    int nonce = find_nonce(block, difficulty);
    block["nonce"] = nonce;
    std::cout << "Found nonce " << nonce << " with hash " << sha256(block.dump()) << '\n';
    json solution;
    solution["nonce"] = nonce;
    std::cout << "Submitting...\n";

    std::cout << net::PostSolution(problem, solution) << '\n';
    return 0;
}
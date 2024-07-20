#include <openssl/evp.h>
#include <openssl/pem.h>
#include <tools.hpp>
#include <iostream>

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
int main() {
    auto statement = tools::GetStatement("tales_of_ssl");
    auto data = statement["required_data"];
    std::string key = statement["private_key"];
    key = "-----BEGIN RSA PRIVATE KEY-----\n" + key;
    key += "\n-----END RSA PRIVATE KEY-----";
    BIO *bio_read = BIO_new(BIO_s_mem());
    BIO_write(bio_read, key.data(), key.size());
    EVP_PKEY *pkey = nullptr;
    PEM_read_bio_PrivateKey(bio_read, &pkey, nullptr, nullptr);
    if (!pkey) {
        std::cout << "Error reading key!\n";
    }
    X509 *certificate = X509_new();

    std::string serial = data["serial_number"];
    std::string country = data["country"];
    std::string code;
    for (auto c : country) {
        if (isupper(c)) {
            code += c;
        }
    }
    std::string domain = data["domain"];

    uint64_t serial_number = std::stoul(serial, 0, 16);

    ASN1_INTEGER_set(X509_get_serialNumber(certificate), serial_number);
    X509_gmtime_adj(X509_get_notBefore(certificate), 0);             // now
    X509_gmtime_adj(X509_get_notAfter(certificate), 3600);  // accepts secs
    X509_set_pubkey(certificate, pkey);

    X509_NAME *name = X509_get_subject_name(certificate);

    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)code.data(), -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)domain.data(), -1, -1, 0);

    X509_set_issuer_name(certificate, name);

    X509_sign(certificate, pkey, EVP_sha1());

    unsigned char *buf = nullptr;
    int len = i2d_X509(certificate, &buf);

    std::string private_key = std::string((char *)buf, len);
    std::ofstream of("cert.crt");
    of << private_key;
    private_key = Encode(private_key);
    nlohmann::json solution;
    solution["certificate"] = private_key;
    tools::PostSolution("tales_of_ssl", solution);
    EVP_PKEY_free(pkey);
    BIO_free(bio_read);
    X509_free(certificate);
    free(buf);
}
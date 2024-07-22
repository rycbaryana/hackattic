#include <openssl/evp.h>
#include <openssl/pem.h>
#include <tools.hpp>
#include <iostream>

int main() {
    auto statement = net::GetStatement("tales_of_ssl");
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
    private_key = base64::Encode(private_key);
    nlohmann::json solution;
    solution["certificate"] = private_key;
    net::PostSolution("tales_of_ssl", solution);
    EVP_PKEY_free(pkey);
    BIO_free(bio_read);
    X509_free(certificate);
    free(buf);
}
#ifndef CRYPTO_HPP
#define CRYPTO_HPP

#include <string>
#include <vector>

namespace crypto {
    void expandKey(const std::string &password, std::vector<unsigned char> &key);
    void xorBuffer(std::vector<unsigned char> &buffer, const std::vector<unsigned char> &key);

    bool encryptFile(const std::string &filePath, const std::string &password);
    bool decryptOnce(const std::string &filePath, const std::string &password, std::vector<unsigned char> &output);
}

#endif

#include "crypto.hpp"
#include "fileutils.hpp"
#include "metadata.hpp"

#include <vector>
#include <string>
#include <sstream>

namespace crypto {

    void expandKey(const std::string &password, std::vector<unsigned char> &key) {
        key.clear();
        if (password.empty()) {
            return;
        }
        std::size_t i;
        for (i = 0; i < password.size(); i++) {
            key.push_back(static_cast<unsigned char>(password[i]));
        }
    }

    void xorBuffer(std::vector<unsigned char> &buffer, const std::vector<unsigned char> &key) {
        if (buffer.empty() || key.empty()) {
            return;
        }

        std::size_t keySize = key.size();
        std::size_t i;
        for (i = 0; i < buffer.size(); i++) {
            buffer[i] = static_cast<unsigned char>(buffer[i] ^ key[i % keySize]);
        }
    }

    bool encryptFile(const std::string &filePath, const std::string &password) {
        std::vector<unsigned char> buffer;
        if (!fileutils::readBinaryFile(filePath, buffer)) {
            return false;
        }

        int layers = 0;
        std::vector<unsigned char> body;

        if (metadata::isEncrypted(buffer)) {
            layers = metadata::getLayers(buffer);
            if (layers < 0) {
                layers = 0;
            }
            layers = layers + 1;
            metadata::removeHeader(buffer);
            body = buffer;
        } else {
            layers = 1;
            body = buffer;
        }

        std::vector<unsigned char> key;
        expandKey(password, key);
        xorBuffer(body, key);

        std::ostringstream oss;
        oss << metadata::HEADER << layers << "\n";
        std::string headerStr = oss.str();

        std::vector<unsigned char> finalBuffer;
        finalBuffer.reserve(headerStr.size() + body.size());

        std::size_t i;
        for (i = 0; i < headerStr.size(); i++) {
            finalBuffer.push_back(static_cast<unsigned char>(headerStr[i]));
        }
        for (i = 0; i < body.size(); i++) {
            finalBuffer.push_back(body[i]);
        }

        if (!fileutils::writeBinaryFile(filePath, finalBuffer)) {
            return false;
        }

        return true;
    }

    bool decryptOnce(const std::string &filePath, const std::string &password, std::vector<unsigned char> &output) {
        output.clear();

        std::vector<unsigned char> buffer;
        if (!fileutils::readBinaryFile(filePath, buffer)) {
            return false;
        }

        if (!metadata::isEncrypted(buffer)) {
            return false;
        }

        metadata::removeHeader(buffer);

        std::vector<unsigned char> key;
        expandKey(password, key);
        xorBuffer(buffer, key);

        output = buffer;
        return true;
    }

}

#include "metadata.hpp"

#include <vector>
#include <string>
#include <cctype>
#include <sstream>

namespace {

    int findHeaderEnd(const std::vector<unsigned char> &buffer) {
        std::size_t headerSize = metadata::HEADER.size();
        if (buffer.size() < headerSize + 2) {
            return -1;
        }

        std::size_t i;
        for (i = headerSize; i < buffer.size(); i++) {
            if (buffer[i] == '\n') {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

}

namespace metadata {

    const std::string HEADER = "MEJINENC|layers=";

    bool isEncrypted(const std::vector<unsigned char> &buffer) {
        std::size_t headerSize = HEADER.size();
        if (buffer.size() < headerSize + 2) {
            return false;
        }

        std::size_t i;
        for (i = 0; i < headerSize; i++) {
            if (buffer[i] != static_cast<unsigned char>(HEADER[i])) {
                return false;
            }
        }

        int endIdx = findHeaderEnd(buffer);
        if (endIdx == -1) {
            return false;
        }

        for (i = headerSize; i < static_cast<std::size_t>(endIdx); i++) {
            if (!std::isdigit(static_cast<unsigned char>(buffer[i]))) {
                return false;
            }
        }

        return true;
    }

    int getLayers(const std::vector<unsigned char> &buffer) {
        if (!isEncrypted(buffer)) {
            return 0;
        }

        std::size_t headerSize = HEADER.size();
        int endIdx = findHeaderEnd(buffer);
        if (endIdx == -1) {
            return 0;
        }

        std::string numberStr;
        std::size_t i;
        for (i = headerSize; i < static_cast<std::size_t>(endIdx); i++) {
            numberStr.push_back(static_cast<char>(buffer[i]));
        }

        std::istringstream iss(numberStr);
        int value = 0;
        iss >> value;
        if (iss.fail()) {
            return 0;
        }
        return value;
    }

    void setLayers(std::vector<unsigned char> &buffer, int newLayers) {
        if (newLayers < 0) {
            newLayers = 0;
        }

        std::vector<unsigned char> body = buffer;
        if (isEncrypted(buffer)) {
            removeHeader(body);
        }

        std::ostringstream oss;
        oss << HEADER << newLayers << "\n";
        std::string headerStr = oss.str();

        std::vector<unsigned char> result;
        result.reserve(headerStr.size() + body.size());

        std::size_t i;
        for (i = 0; i < headerStr.size(); i++) {
            result.push_back(static_cast<unsigned char>(headerStr[i]));
        }
        for (i = 0; i < body.size(); i++) {
            result.push_back(body[i]);
        }

        buffer.swap(result);
    }

    void removeHeader(std::vector<unsigned char> &buffer) {
        if (!isEncrypted(buffer)) {
            return;
        }

        int endIdx = findHeaderEnd(buffer);
        if (endIdx == -1) {
            return;
        }

        std::size_t removeUntil = static_cast<std::size_t>(endIdx + 1);
        if (removeUntil > buffer.size()) {
            return;
        }

        buffer.erase(buffer.begin(), buffer.begin() + static_cast<std::ptrdiff_t>(removeUntil));
    }

}

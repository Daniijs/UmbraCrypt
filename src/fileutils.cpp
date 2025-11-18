#include "fileutils.hpp"

#include <fstream>
#include <vector>
#include <string>

namespace fileutils {

    bool fileExists(const std::string &filePath) {
        std::ifstream file(filePath.c_str(), std::ios::binary);
        return file.good();
    }

    bool readBinaryFile(const std::string &filePath, std::vector<unsigned char> &buffer) {
        buffer.clear();
        std::ifstream file(filePath.c_str(), std::ios::binary);
        if (!file.is_open()) {
            return false;
        }

        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        if (fileSize < 0) {
            file.close();
            return false;
        }
        file.seekg(0, std::ios::beg);

        buffer.resize(static_cast<std::size_t>(fileSize));
        if (fileSize > 0) {
            file.read(reinterpret_cast<char*>(&buffer[0]), fileSize);
        }

        file.close();
        return true;
    }

    bool writeBinaryFile(const std::string &filePath, const std::vector<unsigned char> &buffer) {
        std::ofstream file(filePath.c_str(), std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            return false;
        }

        if (!buffer.empty()) {
            file.write(reinterpret_cast<const char*>(&buffer[0]),
                       static_cast<std::streamsize>(buffer.size()));
        }

        file.close();
        return true;
    }

    bool copyFile(const std::string &src, const std::string &dst) {
        std::vector<unsigned char> buffer;
        if (!readBinaryFile(src, buffer)) {
            return false;
        }
        if (!writeBinaryFile(dst, buffer)) {
            return false;
        }
        return true;
    }

}

#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>
#include <vector>

namespace fileutils {
    bool fileExists(const std::string &filePath);

    bool readBinaryFile(const std::string &filePath, std::vector<unsigned char> &buffer);
    bool writeBinaryFile(const std::string &filePath, const std::vector<unsigned char> &buffer);

    bool copyFile(const std::string &src, const std::string &dst);
}

#endif


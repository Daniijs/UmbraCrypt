#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>
#include <vector>

namespace metadata {

    extern const std::string HEADER;

    bool isEncrypted(const std::vector<unsigned char> &buffer);
    int getLayers(const std::vector<unsigned char> &buffer);
    void setLayers(std::vector<unsigned char> &buffer, int newLayers);
    void removeHeader(std::vector<unsigned char> &buffer);

}

#endif

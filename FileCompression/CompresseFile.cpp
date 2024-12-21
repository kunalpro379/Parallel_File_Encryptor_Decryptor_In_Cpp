// FileCompression/CompresseFile.cpp
#include "CompresseFile.hpp"
#include <zlib.h>
#include <stdexcept>

std::vector<char> FileCompression::compress(const std::string& data) {
    z_stream zs;
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;

    if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
        throw std::runtime_error("Failed to initialize compression");
    }

    zs.avail_in = data.size();
    zs.next_in = (Bytef*)data.data();

    int ret;
    char outbuffer[32768];
    std::vector<char> compressed;

    do {
        zs.avail_out = sizeof(outbuffer);
        zs.next_out = (Bytef*)outbuffer;

        ret = deflate(&zs, Z_FINISH);
        if (compressed.size() < zs.total_out) {
            compressed.insert(compressed.end(), outbuffer, outbuffer + (zs.total_out - compressed.size()));
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        throw std::runtime_error("Error during compression");
    }

    return compressed;
}

std::string FileCompression::decompress(const std::vector<char>& data) {
    z_stream zs;
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    zs.avail_in = data.size();
    zs.next_in = (Bytef*)data.data();

    if (inflateInit(&zs) != Z_OK) {
        throw std::runtime_error("Failed to initialize decompression");
    }

    char outbuffer[32768];
    std::string decompressed;

    do {
        zs.avail_out = sizeof(outbuffer);
        zs.next_out = (Bytef*)outbuffer;

        int ret = inflate(&zs, 0);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&zs);
            throw std::runtime_error("Error during decompression");
        }

        decompressed.append(outbuffer, sizeof(outbuffer) - zs.avail_out);
    } while (zs.avail_out == 0);

    inflateEnd(&zs);
    return decompressed;
}

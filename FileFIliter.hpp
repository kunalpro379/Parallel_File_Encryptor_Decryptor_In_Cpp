#ifndef FILE_FILTER_HPP
#define FILE_FILTER_HPP
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <chrono>

namespace fs = std::filesystem;

class FileFilter {
private:
    std::vector<std::string> skipExtensions;
    std::vector<std::string> allowedExtensions;
    uintmax_t maxFileSize;
    uintmax_t minFileSize;
    std::chrono::system_clock::time_point modifiedAfter;
    bool useWhitelist;

public:
    FileFilter() : 
        maxFileSize(std::numeric_limits<uintmax_t>::max()),
        minFileSize(0),
        modifiedAfter(std::chrono::system_clock::from_time_t(0)),
        useWhitelist(false) {
        
        // Default extensions to skip
        skipExtensions = {
            ".exe", ".dll", ".sys", ".bat", ".cmd", 
            ".msi", ".bin", ".iso", ".img"
        };
    }

    bool shouldProcess(const fs::path& filepath) const {
        try {
            // Check if file exists
            if (!fs::exists(filepath)) {
                std::cout << "File does not exist: " << filepath << std::endl;
                return false;
            }

            // Check if it's a regular file
            if (!fs::is_regular_file(filepath)) {
                std::cout << "Not a regular file: " << filepath << std::endl;
                return false;
            }

            // Get file extension in lowercase
            std::string extension = filepath.extension().string();
            std::transform(extension.begin(), extension.end(), 
                         extension.begin(), ::tolower);

            // Check whitelist if enabled
            if (useWhitelist) {
                bool allowed = false;
                for (const auto& ext : allowedExtensions) {
                    if (extension == ext) {
                        allowed = true;
                        break;
                    }
                }
                if (!allowed) {
                    std::cout << "File extension not in whitelist: " << filepath << std::endl;
                    return false;
                }
            }
            // Check blacklist
            else {
                for (const auto& ext : skipExtensions) {
                    if (extension == ext) {
                        std::cout << "File extension in blacklist: " << filepath << std::endl;
                        return false;
                    }
                }
            }

            // Check file size
            uintmax_t fileSize = fs::file_size(filepath);
            if (fileSize > maxFileSize || fileSize < minFileSize) {
                std::cout << "File size out of bounds: " << filepath << std::endl;
                return false;
            }

            // Check modification time (C++17 compatible)
            auto lastModified = fs::last_write_time(filepath);
            auto lastModifiedTime = std::chrono::system_clock::now();
            // Convert file_time_type to system_clock time_point
            auto sctp = std::chrono::system_clock::time_point(
                std::chrono::duration_cast<std::chrono::system_clock::duration>(
                    lastModified.time_since_epoch()
                )
            );
            
            if (sctp < modifiedAfter) {
                std::cout << "File too old: " << filepath << std::endl;
                return false;
            }

            return true;
        }
        catch (const std::exception& e) {
            std::cout << "Error processing file " << filepath << ": " << e.what() << std::endl;
            return false;
        }
    }

    // Setters
    void setSkipExtensions(const std::vector<std::string>& extensions) {
        skipExtensions = extensions;
    }

    void setAllowedExtensions(const std::vector<std::string>& extensions) {
        allowedExtensions = extensions;
        useWhitelist = true;
    }

    void setMaxFileSize(uintmax_t size) {
        maxFileSize = size;
    }

    void setMinFileSize(uintmax_t size) {
        minFileSize = size;
    }

    void setModifiedAfter(const std::chrono::system_clock::time_point& time) {
        modifiedAfter = time;
    }

    // Utility method to format file size
    static std::string formatFileSize(uintmax_t bytes) {
        const char* suffixes[] = {"B", "KB", "MB", "GB", "TB"};
        int suffixIndex = 0;
        double size = static_cast<double>(bytes);

        while (size >= 1024 && suffixIndex < 4) {
            size /= 1024;
            suffixIndex++;
        }

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << size << " " << suffixes[suffixIndex];
        return ss.str();
    }
};

#endif
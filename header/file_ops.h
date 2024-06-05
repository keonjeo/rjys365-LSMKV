#pragma once

#include<string>
#include<sys/stat.h>

namespace file_ops{
    inline bool isFile(const std::string &file_path) {
        struct stat stat_buf;
        if (stat(file_path.c_str(), &stat_buf) != 0) {
            return false;
        }
        return S_ISREG(stat_buf.st_mode);
    }

    inline off_t getFileSize(const std::string &file_path) {
        struct stat stat_buf;
        if (stat(file_path.c_str(), &stat_buf) != 0) {
            return -1;
        }
        return stat_buf.st_size;
    }

    inline bool isDir(const std::string &file_path) {
        struct stat stat_buf;
        if (stat(file_path.c_str(), &stat_buf) != 0) {
            return false;
        }
        return S_ISDIR(stat_buf.st_mode);
    }
}
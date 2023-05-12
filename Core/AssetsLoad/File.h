//
// Created by SilverAsh on 2023/4/29.
//

#ifndef OPENGL_FILE_H
#define OPENGL_FILE_H

#include "LOG.h"
#include "filesystem"
#include <cstdio>
#include <cassert>
#include <ios>
#include <fstream>
#include <string>

#define AssertPath(path) assert(std::filesystem::exists(path)==true)

class File
{
public:
    static bool isExits(const std::string &path) {
        //还有其他方法，这个是c++17的标准
        return std::filesystem::exists(path);
    }

    static std::string LoadFile(const std::string &path) {
        std::ifstream file(path.c_str());
        if (!file) {
            std::cerr << "Failed to open file:" + path
                      << std::endl;
            return "";
        }

        std::string result;
        std::string line;
        while (std::getline(file, line)) {
            result += line;
        }
        return std::move(result);
    }

};


#endif //OPENGL_FILE_H

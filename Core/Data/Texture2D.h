//
// Created by SilverAsh on 2023/4/17.
//

#ifndef LEARNOPENGL_TEXTURE2D_H
#define LEARNOPENGL_TEXTURE2D_H

#include <unordered_map>
#include "iostream"
#include "string"
#include "glad/glad.h"

class Texture2D
{
public:
    int          height;
    int          width;
    GLuint       format;
    unsigned int ID;
    std::string  type;

    Texture2D() {}

    void free();
};

#endif //LEARNOPENGL_TEXTURE2D_H


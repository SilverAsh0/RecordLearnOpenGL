//
// Created by SilverAsh on 2023/4/29.
//

#ifndef OPENGL_LOG_H
#define OPENGL_LOG_H

#include "iostream"

#define LOG(msg) std::cout<<msg<<std::endl;
#define FILE_DONT_EXISTS(msg) std::cout<<"Failed to load file maybe it didn't exist :"<<msg<<std::endl;

#endif //OPENGL_LOG_H

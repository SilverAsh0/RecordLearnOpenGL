//
// Created by SilverAsh on 2023/4/26.
//

#ifndef OPENGL_MATRIX4X4_H
#define OPENGL_MATRIX4X4_H

#include "Vector3.h"

namespace Math
{
    //GPU需要矩阵按列存储
    class Matrix4x4
    {
        int data[4][4] = {0};

        Matrix4x4() {
            data[0][0] = 1;
            data[1][1] = 1;
            data[2][2] = 1;
            data[3][3] = 1;
        }

        Matrix4x4(Vector3 v1) {}

        Matrix4x4 operator*(const Vector3 &vec3) {

        }

    };
}


#endif //OPENGL_MATRIX4X4_H

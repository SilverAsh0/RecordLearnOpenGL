//
// Created by SilverAsh on 2023/4/18.
//

#ifndef LEARNOPENGL_VECTOR3_H
#define LEARNOPENGL_VECTOR3_H

#include "iostream"

namespace Math
{
    class Vector3
    {
    public:
        float          x = 0;
        float          y = 0;
        float          z = 0;
        static Vector3 zero;
        static Vector3 one;
        static Vector3 forward;
        static Vector3 left;
        static Vector3 up;
        float operator[](int index);
        Vector3 operator+(const Vector3 &vec3);
        Vector3 operator*(const Vector3 &vec3);
        Vector3 operator-(const Vector3 &vec3);
        Vector3 operator/(const Vector3 &vec3);
        static Vector3 normalize();

        float getLength();

        float getSqure();

        friend std::ostream &operator<<(std::ostream &os, const Vector3 &vec3) {
            os << "(" << vec3.x << "," << vec3.y << "," << vec3.z << ")";
            return os;
        }
    };

    static Vector3 cross(const Vector3 &vec1, const Vector3 &vec2);
    static float dot(const Vector3 &vec1, const Vector3 &vec2);

}


#endif //LEARNOPENGL_VECTOR3_H

//
// Created by SilverAsh on 2023/4/26.
//

#ifndef OPENGL_POINTLIGHT_H
#define OPENGL_POINTLIGHT_H

#include "glm/glm.hpp"
#include "Light.h"

class PointLight : public Light
{
public:
    float radius = 50.f;

    float constant;
    float linear;
    float quadratic;

    PointLight() {
        ambient = glm::vec3(0.4f, 0.4f, .4f);
    }
};


#endif //OPENGL_POINTLIGHT_H

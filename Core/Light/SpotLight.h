//
// Created by SilverAsh on 2023/4/26.
//

#ifndef OPENGL_SPOTLIGHT_H
#define OPENGL_SPOTLIGHT_H

#include "glm/glm.hpp"
#include "Light.h"

class SpotLight : public Light
{
public:
    glm::vec3 direction;

    float     constant;
    float     linear;
    float     quadratic;

    float     innerCutOff = glm::cos(glm::radians(10.5f));
    float     outerCutOff = glm::cos(glm::radians(15.5f));

    SpotLight() {
        ambient = glm::vec3(0.1f, 0.1f, 0.1f);
        diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    }

};


#endif //OPENGL_SPOTLIGHT_H

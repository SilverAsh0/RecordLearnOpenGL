//
// Created by SilverAsh on 2023/4/26.
//

#ifndef OPENGL_DIRLIGHT_H
#define OPENGL_DIRLIGHT_H

#include "Light.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "Math/Vector3.h"

class DirLight : public Light
{
public:
    glm::vec3 &direction;

    DirLight() : direction(transform.FORWARD) {
        transform.position   = glm::vec3(0, 5, -10);
        transform.eulerAngle = glm::vec3(45, 0, 0);
        transform.update_model_and_axis();
    }

    DirLight(glm::vec3 position, glm::vec3 eulerAngle) : direction(transform.FORWARD) {
        transform.position   = position;
        transform.eulerAngle = eulerAngle;
        transform.update_model_and_axis();
    }

};


#endif //OPENGL_DIRLIGHT_H

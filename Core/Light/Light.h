//
// Created by SilverAsh on 2023/4/25.
//

#ifndef OPENGL_LIGHT_H
#define OPENGL_LIGHT_H

#include "Class/Transform.h"
#include "glm/ext/matrix_clip_space.hpp"

class Light
{
public:
    Transform transform;
    glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);

    const glm::mat4 &get_view() {
        view = glm::lookAt(transform.position, transform.position + transform.forward, transform.up);
        return view;
    }

    const glm::mat4 &get_perspective() {
        float near_plane = 1.0f, far_plane = 7.5f;

        perspective = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 0.1f, 30.f);
        //perspective = glm::perspective(glm::radians(80 / 2.f), 16/float (10), 0.1f, 40.f);
        return perspective;
    }

private:
    glm::mat4 view;
    glm::mat4 perspective;
};


#endif //OPENGL_LIGHT_H

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include "Transform.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

class Camera
{
public:
    // camera Attributes
    Transform transform;
    float     zNear            = 0.1f;
    float     zFar             = 200.f;
    float     fov              = 80.f;
    float     aspect           = float(SCR_WIDTH) / float(SCR_HEIGHT);
    float     MovementSpeed    = 3;
    float     MouseSensitivity = 0.1f;
    float     Zoom;
    glm::vec3 &position;

    Camera(const glm::vec3 &vec3) : position(transform.position) {
        position = vec3;
    }

    const glm::mat4 &get_view() {
        cal_view();
        return view;
    }

    const glm::mat4 &get_perspective() {
        cal_perspective();
        return perspective;
    }

    glm::vec3 getLookAt() {
        return -transform.forward;
    }

    void free_move(GLFWwindow *window, float delta);
    void free_rotate(float yOffset, float xOffset);
    void free_zoom();
private:
    glm::mat4 view;
    glm::mat4 perspective;
    void cal_view();
    void cal_perspective();
};

#endif
#include "Camera.h"
#include "Transform.h"
#include "iostream"

void Camera::cal_view() {
    //TODO:自己实现
    transform.update_model_and_axis();
    view = glm::lookAt(transform.position, transform.position - transform.forward, Transform::UP);
}

void Camera::cal_perspective() {
    perspective = glm::perspective(glm::radians(fov / 2), aspect, zNear, zFar);
}

void Camera::free_move(GLFWwindow *window, float delta) {

    glm::vec3 moveDir(0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveDir = -transform.forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveDir = transform.forward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveDir = -transform.left;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveDir = transform.left;
    transform.position += moveDir * delta * MovementSpeed;
}

void Camera::free_rotate(float yOffset, float xOffset) {
    transform.eulerAngle.x += xOffset * MouseSensitivity;
    transform.eulerAngle.y += yOffset * MouseSensitivity;

    if (transform.eulerAngle.x > 89.0f)
        transform.eulerAngle.x = 89.0f;
    if (transform.eulerAngle.x < -89.0f)
        transform.eulerAngle.x = -89.0f;

}

void Camera::free_zoom() {

}

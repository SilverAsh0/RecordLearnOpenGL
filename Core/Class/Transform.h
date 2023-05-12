//
// Created by SilverAsh on 2023/4/17.
//

#ifndef LEARNOPENGL_TRANSFORM_H
#define LEARNOPENGL_TRANSFORM_H

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

class Transform
{
//TODO:使用四元数描述旋转
public:
    glm::vec3 position   = glm::vec3(0.f);
    glm::vec3 eulerAngle = glm::vec3(0.f);
    glm::vec3 scale      = glm::vec3(1.f);

    static glm::vec3 FORWARD;
    static glm::vec3 LEFT;
    static glm::vec3 UP;

    //使用右手坐标系
    //z轴
    glm::vec3 forward = FORWARD;
    //x轴
    glm::vec3 left    = LEFT;
    //y轴
    glm::vec3 up      = UP;

    Transform() {
        forward = FORWARD;
        left    = LEFT;
        up      = UP;
    }

    Transform(glm::vec3 pos) : position(pos) {}

    Transform(glm::vec3 pos, glm::vec3 eulerAngle, glm::vec3 scale) : position(pos), eulerAngle(eulerAngle),
                                                                      scale(scale) {}

    void update_model_and_axis();

    const glm::mat4 &get_model() {
        update_model_and_axis();
        return model;
    }

private:
    glm::mat4 model = glm::mat4(0.f);


};

#endif //LEARNOPENGL_TRANSFORM_H

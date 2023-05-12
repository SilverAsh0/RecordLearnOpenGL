//
// Created by SilverAsh on 2023/4/17.
//

#include "Transform.h"
#include "glm/gtc/matrix_transform.hpp"

glm::vec3 Transform::FORWARD = glm::vec3(0, 0, 1);
glm::vec3 Transform::LEFT    = glm::vec3(1, 0, 0);
glm::vec3 Transform::UP      = glm::vec3(0, 1, 0);

void Transform::update_model_and_axis() {

    glm::mat4 translation = glm::mat4(1.0f);
    translation = glm::translate(translation, position);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(scale, this->scale);

    //glmʹ�õ����������ҳ�

    //��ZYX����ת˳��
    //Ϊ��ʵ���������ŷ������ת�����������������������ת
    glm::mat4 rotation(1.0f);
    rotation = glm::rotate(rotation, glm::radians(eulerAngle.z), glm::vec3(0, 0, 1));
    rotation = glm::rotate(rotation, glm::radians(eulerAngle.y), glm::vec3(0, 1, 0));
    rotation = glm::rotate(rotation, glm::radians(eulerAngle.x), glm::vec3(1, 0, 0));


    //��������ϵά��
    forward = rotation * glm::vec4(0, 0, 1, 0);
    left    = rotation * glm::vec4(1, 0, 0, 0);
    up      = rotation * glm::vec4(0, 1, 0, 0);

    model = translation * scale * rotation;

}

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Light/Light.h"
#include "Light/SpotLight.h"
#include "Light/DirLight.h"
#include "Light/PointLight.h"
class Shader
{
public:
    unsigned int ID;

    Shader() {}

    void use() const {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setDirLight(const std::string &name, const DirLight &dirLight) const {
        setVec3(name + ".direction", dirLight.direction);
        setVec3(name + ".ambient", dirLight.ambient);
        setVec3(name + ".diffuse", dirLight.diffuse);
    }

    void setPointLight(const std::string &name, const PointLight &pointLight) const {
        setVec3(name + ".position", pointLight.transform.position);
        setVec3(name + ".ambient", pointLight.ambient);
        setVec3(name + ".diffuse", pointLight.diffuse);
    }

    void setSpotLight(std::string name, const SpotLight &spotLight) const {
        name = "spotLight";
        setVec3(name + ".position", spotLight.transform.position);
        setVec3(name + ".direction", spotLight.direction);
        setVec3(name + ".ambient", spotLight.ambient);
        setVec3(name + ".diffuse", spotLight.diffuse);
        setFloat(name + ".innerCutOff", spotLight.innerCutOff);
        setFloat(name + ".outerCutOff", spotLight.outerCutOff);
    }


private:

};

#endif
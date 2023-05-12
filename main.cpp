#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AssetsLoad/AssetsLoad.h"
#include "Class/Transform.h"
#include "Data/CubeMap.h"
#include "Data/Shader.h"

#include <iostream>

#include "Data/Mesh.h"
#include "Light/Light.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "Light/SpotLight.h"
#include "Light/DirLight.h"
#include "Light/PointLight.h"



//if(lightVector.w == 0.0)
//
//else if(lightVector.w == 1.0)
//

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processKeyInput(GLFWwindow *window);
void init();
void shaderMapPass();

GLFWwindow         *window;
const unsigned int SCR_WIDTH  = 1600;
const unsigned int SCR_HEIGHT = 1000;
glm::vec3          FORWARD    = glm::vec3(0, 0, 1);
glm::vec3          UP         = glm::vec3(0, 1, 0);
glm::vec3          LEFT       = glm::vec3(1, 0, 0);
Camera             camera(glm::vec3(0, 0, 10));
float              lastX      = SCR_WIDTH / 2.0f;
float              lastY      = SCR_HEIGHT / 2.0f;
bool               firstMouse = true;

const std::string assetsPath = "D:/ClionProject/OpenGL/Assets/";
const std::string shaderPath = "D:/ClionProject/OpenGL/Assets/Shader/";

float deltaTime = 0.0f;
float lastFrame = 0.0f;

DirLight   dirLight;
PointLight pointLight;
SpotLight  spotLight;

int main() {
    init();
    //TODO:不同类型的模型可能需要不同的filp？？？
    //stbi_set_flip_vertically_on_load(false);

    Shader skyBoxShader = AssetsLoad::LoadShader(shaderPath + "SkyBoxShader");
    SkyBox skyBox(assetsPath + "SkyBox");

    Shader defaultShader = AssetsLoad::LoadShader(shaderPath + "Shadow");
    Shader normalShader  = AssetsLoad::LoadShader(shaderPath + "showNormal", true);
    Model  &character    = *AssetsLoad::LoadObj(assetsPath + "nanosuit/nanosuit.obj");
    Model  &floor        = *AssetsLoad::LoadObj(assetsPath + "floor.obj");
    Model  &cube         = *AssetsLoad::LoadObj(assetsPath + "Cube.obj");
    Model  &quad         = *AssetsLoad::LoadObj(assetsPath + "Quad.obj");

    Shader depthShader   = AssetsLoad::LoadShader(shaderPath + "depthMap");
    Shader renderTexture = AssetsLoad::LoadShader(shaderPath + "outRenderTexture");

    //申请缓冲区
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    //申请深度附件
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    //指定纹理附件
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    //显式告诉OpenGL不需要颜色缓冲附件
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is not complete!" << std::endl;
    }

    while (!glfwWindowShouldClose(window)) {

        //每一帧的逻辑更新
        processKeyInput(window);
        glfwPollEvents();
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.transform.update_model_and_axis();

        //每一帧的渲染更新
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_MULTISAMPLE);

//        glClearColor(0.0f, 0.0f, 0.f, .0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //以方向光的坐标系渲染深度图
        depthShader.use();
        depthShader.setMat4("spaceMatrix", dirLight.get_perspective() * dirLight.get_view());
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        cube.Draw(depthShader);
        character.Draw(depthShader);
        floor.transform.eulerAngle = glm::vec3(90, 0, 0);
        floor.Draw(depthShader);

        //绑定默认缓冲区
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //将深度图显示在屏幕上
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        renderTexture.use();
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, depthMap);
//        renderTexture.setInt("depthMap", 0);
//        quad.Draw(renderTexture);

        //模型绘制，MVP矩阵由Draw函数上传，直接光也由Draw函数上传，循环中只需要设置其他变量即可
        //同时shader中的贴图除非明确指定，否则需要设置为默认名称
        defaultShader.use();
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        defaultShader.setInt("shadowMap", 1);
        defaultShader.setVec3("viewPos", camera.position);
        defaultShader.setMat4("lightSpaceMatrix", dirLight.get_perspective() * dirLight.get_view());

        //draw character
        character.Draw(defaultShader);
        //draw cube
        cube.Draw(defaultShader);
        //draw floor
        floor.Draw(defaultShader);
//
//        //天空盒绘制
//        skyBoxShader.use();
//        skyBox.Draw(skyBoxShader);

        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}

void shadowMapPass() {

}

void init() {
#pragma region init
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        //return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1;
    }
#pragma endregion
}

void processKeyInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    camera.free_move(window, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);


    if (firstMouse) {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    float xOffset = -ypos + lastY;

    float yOffset = -xpos + lastX;

    lastX = xpos;
    lastY = ypos;

    camera.free_rotate(yOffset, xOffset);

}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    //camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
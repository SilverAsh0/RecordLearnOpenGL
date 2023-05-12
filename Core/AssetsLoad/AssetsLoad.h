//
// Created by SilverAsh on 2023/4/29.
//

#ifndef OPENGL_ASSETSLOAD_H
#define OPENGL_ASSETSLOAD_H


#include "Data/CubeMap.h"
#include "Data/Mesh.h"
#include "Data/Shader.h"
#include "Data/Texture2D.h"
#include "string"
#include <string>
#include <unordered_map>
#include <assimp/scene.h>

//加载stb_image前需要这句话!!!!
#define STB_IMAGE_IMPLEMENTATION

class Model;

class Mesh;

class Shader;

class Texture2D;

class CubeMap;

class AssetsLoad
{
public:
    static std::unordered_map<std::string, Model *>     models;
    static std::unordered_map<std::string, Shader *>    shaders;
    static std::unordered_map<std::string, Texture2D *> textures;
    static std::unordered_map<std::string, CubeMap *>   cubeMaps;

    static Model *LoadObj(const std::string &path);

    static Texture2D *LoadTexture2D(const std::string &path, const std::string &type);

    static CubeMap *LoadCubeMap(const std::string &path);

    static Shader & LoadShader(const std::string &path);

    static Shader & LoadShader(const std::string &path, bool isGeometry);
private:
    static std::string modelDirPath;
    static void checkCompileErrors(GLuint shader, std::string type, std::string name);
    static Mesh *processMesh(aiMesh *mesh, const aiScene *scene);
    static std::vector<Texture2D *> processTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);
};


#endif //OPENGL_ASSETSLOAD_H

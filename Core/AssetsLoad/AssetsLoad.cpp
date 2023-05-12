//
// Created by SilverAsh on 2023/4/29.
//

#include "AssetsLoad.h"
#include "Data/CubeMap.h"
#include "Data/Mesh.h"
#include "Data/Shader.h"
#include "Data/Texture2D.h"
#include "LOG.h"
#include "assimp/scene.h"
#include "stb_image.h"
#include "File.h"
#include <memory>
#include <queue>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <unordered_map>

std::unordered_map<std::string, Model *>      AssetsLoad::models;
std::unordered_map<std::string, Texture2D *>  AssetsLoad::textures;
std::unordered_map<std::string, Shader *>     AssetsLoad::shaders;
std::unordered_map<std::string, CubeMap *>    AssetsLoad::cubeMaps;
std::string                                   AssetsLoad::modelDirPath;

CubeMap *AssetsLoad::LoadCubeMap(const std::string &path) {

    AssertPath(path);
    if (cubeMaps.count(path)) {
        return cubeMaps[path];
    }

    CubeMap *cubeMap = new CubeMap();

    std::vector<std::string> faces{
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"
    };

    glGenTextures(1, &cubeMap->ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->ID);

    int               width, height, channelCounts, format;
    for (unsigned int i = 0; i < faces.size(); i++) {
        std::string   fin_path = path + '/' + faces[i];
        unsigned char *data    = stbi_load(fin_path.c_str(), &width, &height, &channelCounts, 0);
        //TODO:SRGB文件加载
        switch (channelCounts) {
            case 1:
                format = GL_ALPHA;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        }
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
                         data);
            stbi_image_free(data);
        } else {
            FILE_DONT_EXISTS(fin_path);
            delete (cubeMap);
            stbi_image_free(data);
            return nullptr;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    cubeMaps.insert({path, cubeMap});
    return cubeMap;
}

Texture2D *AssetsLoad::LoadTexture2D(const std::string &path, const std::string &type) {
    if (textures.count(path)) {
        return textures[path];
    }
    int channelCounts;
    AssertPath(path);
    Texture2D     *texture = new Texture2D();
    //加载图片资源
    unsigned char *data    = stbi_load(path.c_str(), &texture->width, &texture->height, &channelCounts, 0);
    if (data == nullptr) {
        FILE_DONT_EXISTS(path);
        delete texture;
        return nullptr;
    }

    texture->type = type;
    //TODO:SRGB加载
    switch (channelCounts) {
        case 1:
            texture->format = GL_ALPHA;
            break;
        case 3:
            texture->format = GL_RGB;
            break;
        case 4:
            texture->format = GL_RGBA;
            break;
    }
    //绑定纹理句柄
    glGenTextures(1, &texture->ID);
    glBindTexture(GL_TEXTURE_2D, texture->ID);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //发送到GPU
    glTexImage2D(GL_TEXTURE_2D, 0, texture->format, texture->width, texture->height, 0, texture->format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    //解绑
    glBindTexture(GL_TEXTURE_2D, 0);
    textures.insert({path, texture});
    return texture;
}

Model *AssetsLoad::LoadObj(const std::string &path) {
    AssertPath(path);

    if (models.count(path))
        return models[path];

    Assimp::Importer import;
    const aiScene    *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return nullptr;
    }

    modelDirPath = path.substr(0, path.find_last_of('/') + 1);
    Model                *model = new Model();
    std::queue<aiNode *> que;
    que.push(scene->mRootNode);
    while (!que.empty()) {
        aiNode *node = que.front();
        que.pop();
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            model->meshes.push_back(processMesh(mesh, scene));

        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            que.push(node->mChildren[i]);
        }
    }
    models.insert({path, model});
    return model;
}

Mesh *AssetsLoad::processMesh(aiMesh *mesh, const aiScene *scene) {
    // data to fill
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D *>  textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex    vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x        = mesh->mVertices[i].x;
        vector.y        = mesh->mVertices[i].y;
        vector.z        = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals()) {
            vector.x      = mesh->mNormals[i].x;
            vector.y      = mesh->mNormals[i].y;
            vector.z      = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        if (mesh->HasTangentsAndBitangents()) {
            // tangent
            vector.x         = mesh->mTangents[i].x;
            vector.y         = mesh->mTangents[i].y;
            vector.z         = mesh->mTangents[i].z;
            vertex.Tangent   = vector;
            // bitangent
            vector.x         = mesh->mBitangents[i].x;
            vector.y         = mesh->mBitangents[i].y;
            vector.z         = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // active models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x            = mesh->mTextureCoords[0][i].x;
            vec.y            = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

        } else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    //process index
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace            face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j    = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    //???default texture;
    //都unsigned int 了，还搁那-1呢
    //插入一个默认贴图,但只有一个,如果不指定贴图，opengl默认给的值全是黑色
    if (mesh->mMaterialIndex == 0) {
        textures.push_back(LoadTexture2D(modelDirPath + "wall.jpg", "texture_diffuse"));
        return new Mesh(vertices, indices, textures);
    }

    // process materials
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Texture2D *> diffuseMaps = processTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture2D *> specularMaps = processTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture2D *> normalMaps = processTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture2D *> heightMaps = processTextures(material, aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return new Mesh(vertices, indices, textures);
}

std::vector<Texture2D *> AssetsLoad::processTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName) {
    std::vector<Texture2D *> textures;
    for (unsigned int        i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        textures.push_back(LoadTexture2D(modelDirPath + str.C_Str(), typeName));
    }
    return textures;
}

Shader &AssetsLoad::LoadShader(const std::string &path) {
    std::string   vertexPath   = path + ".vert";
    std::string   fragmentPath = path + ".frag";
    // 1. retrieve the vertex/fragment source code from filePath
    std::string   vertexCode;
    std::string   fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char    *vShaderCode = vertexCode.c_str();
    const char    *fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int  vertex, fragment;
    std::string   vertexName   = std::string(vertexPath.begin() + vertexPath.find_last_of('/') + 1,
                                             vertexPath.end() - 5);

    std::string fragmentName = std::string(fragmentPath.begin() + fragmentPath.find_last_of('/') + 1,
                                           fragmentPath.end() - 5);

    //TODO:删除失败时的shader指针
    Shader       *shader = new Shader();
    unsigned int &ID     = shader->ID;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX", vertexName);
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT", fragmentName);
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM", vertexName + " " + fragmentName);
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return *shader;
}

Shader &AssetsLoad::LoadShader(const std::string &path, bool isGeometry) {
    //TODO:Shader文件的读取
    std::string vertexPath   = path + ".vert";
    std::string fragmentPath = path + ".frag";
    std::string geometryPath = path + ".geom";

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {

        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        gShaderFile.open(geometryPath);

        std::stringstream vShaderStream, fShaderStream, gShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();
        gShaderFile.close();

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        geometryCode = gShaderStream.str();
    }
    catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char    *vShaderCode = vertexCode.c_str();
    const char    *fShaderCode = fragmentCode.c_str();
    const char    *gShaderCode = geometryCode.c_str();
    // 2. compile shaders
    unsigned int  vertex, fragment, geometry;
    std::string   vertexName   = std::string(vertexPath.begin() + vertexPath.find_last_of('/') + 1,
                                             vertexPath.end() - 5);

    std::string fragmentName = std::string(fragmentPath.begin() + fragmentPath.find_last_of('/') + 1,
                                           fragmentPath.end() - 5);

    std::string  geometryName = std::string(geometryPath.begin() + geometryPath.find_last_of('/') + 1,
                                            geometryPath.end() - 5);
    Shader       *shader      = new Shader();
    unsigned int &ID          = shader->ID;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX", vertexName);
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT", fragmentName);

    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    checkCompileErrors(geometry, "GEOMETRY", geometryName);

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glAttachShader(ID, geometry);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM", vertexName + " " + fragmentName + " " + geometryName);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);
    return *shader;
}

void AssetsLoad::checkCompileErrors(GLuint shader, std::string type, std::string name) {
    GLint  success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                      << "出错文件:" << name
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                      << "出错文件:" << name
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

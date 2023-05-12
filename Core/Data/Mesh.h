#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <unordered_map>
#include <vector>
#include "Class/Transform.h"
#include "Class/camera.h"
#include "Data/CubeMap.h"
#include "Light/DirLight.h"
#include "Shader.h"
#include "Texture2D.h"


extern Camera   camera;
extern DirLight dirLight;
#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int       m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float     m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh
{
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D *>  textures;

    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D *> textures) {
        this->vertices = vertices;
        this->indices  = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(Shader &shader);


private:
    unsigned int VBO, EBO, VAO;

    void setupMesh();

};

class Model
{
public:
    std::vector<Mesh *> meshes;
    Transform           transform;

    Model() {}

    // draws the model, and thus all its meshes
    void Draw(Shader &shader) {
        //set mvp matrix
        shader.setMat4("model", transform.get_model());
        shader.setMat4("view", camera.get_view());
        shader.setMat4("projection", camera.get_perspective());
        //set light
        shader.setDirLight("dirLight", dirLight);
        for (unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i]->Draw(shader);
        }
    }

    ~Model() {

    }

private:
};


#endif
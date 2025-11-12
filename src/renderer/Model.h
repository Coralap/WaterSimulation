#pragma once
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
class Model {
public:
    std::vector<Mesh*> meshes;

    Model() = default;

    void Draw(Shader& shader){
        for(Mesh* mesh : meshes){
            mesh->Draw(shader);
        }

    }
    void AddMesh(Mesh* mesh){meshes.push_back(mesh);}
};

#pragma once
#include <vector>
#include "Shader.h"
#include "Texture.h"
class Mesh {
public:
    std::vector<float> vertices; 
    std::vector<unsigned int> indices;  
    std::string texturePath;
    Texture* texture = nullptr;

    ~Mesh() {
        if (texture) delete texture;
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }


    Mesh(const std::vector<float>& verts, const std::vector<unsigned int>& inds,std::string texturePath)
        : vertices(verts), indices(inds),texturePath(texturePath)
    {
        setupMesh();
    }


    Mesh(const std::vector<float>& verts, const std::vector<unsigned int>& inds)
        : vertices(verts), indices(inds) 
    {
        setupMesh();
    }

    void Draw(Shader& shader){
        shader.use();   

        if(texture!=NULL){
            texture->use();
            shader.setInt("Texture", 0);
        }
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

    void setData(const std::vector<float>& verts, const std::vector<unsigned int>& inds) {
        vertices = verts;
        indices = inds;
        setupMesh(); // re-upload to GPU
    }

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        //position attribute 3 floats
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // normal attribute 3 floats
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // texcoords attribute 2 floats
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        if(!texturePath.empty()){
            texture = new Texture(texturePath);

        }
    }

};
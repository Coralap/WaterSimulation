#pragma once
#include "Shader.h"
#include "Model.h"
#include "../core/Transform.h"
class Entity {
public:
    Model* model;
    Transform transform;
    Entity(Model* model) : model(model) {}

    glm::mat4 getModelMatrix() const {
        return transform.getModelMatrix();
    }

    void Draw(Shader& shader) {
        shader.setMat4("model", getModelMatrix());
        model->Draw(shader);
    }
};

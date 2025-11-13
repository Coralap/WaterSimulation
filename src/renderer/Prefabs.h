#pragma once
#include <vector>
#include <string>
#include <cmath>
#include "./Mesh.h"

namespace Prefabs {

    // Creates a cube mesh with optional texture
    inline Mesh* Cube(const std::string& texturePath = "") {
        std::vector<float> vertices = {
            // positions         // normals          // texcoords
            // back face
            -0.5f,-0.5f,-0.5f,   0.0f,0.0f,-1.0f,   0.0f,0.0f,
             0.5f,-0.5f,-0.5f,   0.0f,0.0f,-1.0f,   1.0f,0.0f,
             0.5f, 0.5f,-0.5f,   0.0f,0.0f,-1.0f,   1.0f,1.0f,
            -0.5f, 0.5f,-0.5f,   0.0f,0.0f,-1.0f,   0.0f,1.0f,

            // front face
            -0.5f,-0.5f, 0.5f,   0.0f,0.0f,1.0f,    0.0f,0.0f,
             0.5f,-0.5f, 0.5f,   0.0f,0.0f,1.0f,    1.0f,0.0f,
             0.5f, 0.5f, 0.5f,   0.0f,0.0f,1.0f,    1.0f,1.0f,
            -0.5f, 0.5f, 0.5f,   0.0f,0.0f,1.0f,    0.0f,1.0f,

            // left face
            -0.5f, 0.5f, 0.5f,  -1.0f,0.0f,0.0f,    1.0f,0.0f,
            -0.5f, 0.5f,-0.5f,  -1.0f,0.0f,0.0f,    1.0f,1.0f,
            -0.5f,-0.5f,-0.5f,  -1.0f,0.0f,0.0f,    0.0f,1.0f,
            -0.5f,-0.5f, 0.5f,  -1.0f,0.0f,0.0f,    0.0f,0.0f,

            // right face
             0.5f, 0.5f, 0.5f,   1.0f,0.0f,0.0f,    1.0f,0.0f,
             0.5f, 0.5f,-0.5f,   1.0f,0.0f,0.0f,    1.0f,1.0f,
             0.5f,-0.5f,-0.5f,   1.0f,0.0f,0.0f,    0.0f,1.0f,
             0.5f,-0.5f, 0.5f,   1.0f,0.0f,0.0f,    0.0f,0.0f,

            // bottom face
            -0.5f,-0.5f,-0.5f,   0.0f,-1.0f,0.0f,   0.0f,1.0f,
             0.5f,-0.5f,-0.5f,   0.0f,-1.0f,0.0f,   1.0f,1.0f,
             0.5f,-0.5f, 0.5f,   0.0f,-1.0f,0.0f,   1.0f,0.0f,
            -0.5f,-0.5f, 0.5f,   0.0f,-1.0f,0.0f,   0.0f,0.0f,

            // top face
            -0.5f, 0.5f,-0.5f,   0.0f,1.0f,0.0f,    0.0f,1.0f,
             0.5f, 0.5f,-0.5f,   0.0f,1.0f,0.0f,    1.0f,1.0f,
             0.5f, 0.5f, 0.5f,   0.0f,1.0f,0.0f,    1.0f,0.0f,
            -0.5f, 0.5f, 0.5f,   0.0f,1.0f,0.0f,    0.0f,0.0f
        };

        std::vector<unsigned int> indices = {
            0,1,2, 2,3,0,        // back
            4,5,6, 6,7,4,        // front
            8,9,10,10,11,8,      // left
            12,13,14,14,15,12,   // right
            16,17,18,18,19,16,   // bottom
            20,21,22,22,23,20    // top
        };

        return new Mesh(vertices, indices, texturePath);
    }

    // Creates a pyramid mesh with optional texture
    inline Mesh* Pyramid(const std::string& texturePath = "") {
        std::vector<float> vertices = {
            // positions          // normals         // texcoords
            -0.5f, 0.0f, -0.5f,   0.0f,-1.0f,0.0f,  0.0f,0.0f,
             0.5f, 0.0f, -0.5f,   0.0f,-1.0f,0.0f,  1.0f,0.0f,
             0.5f, 0.0f,  0.5f,   0.0f,-1.0f,0.0f,  1.0f,1.0f,
            -0.5f, 0.0f,  0.5f,   0.0f,-1.0f,0.0f,  0.0f,1.0f,
             0.0f, 0.8f,  0.0f,   0.0f,1.0f,0.0f,   0.5f,0.5f
        };

        std::vector<unsigned int> indices = {
            0,1,2, 2,3,0, // base
            0,1,4,
            1,2,4,
            2,3,4,
            3,0,4
        };
        if (texturePath == "") {
            return new Mesh(vertices, indices);
        }
        return new Mesh(vertices, indices, texturePath);
    }

    // UV sphere 
    inline Mesh* Sphere(const std::string& texturePath = "", unsigned int sectorCount = 32, unsigned int stackCount = 16, float radius = 0.5f) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        const float PI = 3.14159265359f;

        for (unsigned int i = 0; i <= stackCount; ++i) {
            float stackAngle = PI / 2 - i * (PI / stackCount); // from pi/2 to -pi/2
            float xy = radius * cosf(stackAngle);
            float z = radius * sinf(stackAngle);

            for (unsigned int j = 0; j <= sectorCount; ++j) {
                float sectorAngle = j * (2 * PI / sectorCount);

                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);
                float nx = x / radius;
                float ny = y / radius;
                float nz = z / radius;
                float s = (float)j / sectorCount;
                float t = (float)i / stackCount;

                vertices.insert(vertices.end(), { x, y, z, nx, ny, nz, s, t });
            }
        }

        for (unsigned int i = 0; i < stackCount; ++i) {
            unsigned int k1 = i * (sectorCount + 1);
            unsigned int k2 = k1 + sectorCount + 1;

            for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) indices.insert(indices.end(), { k1, k2, k1 + 1 });
                if (i != (stackCount - 1)) indices.insert(indices.end(), { k1 + 1, k2, k2 + 1 });
            }
        }

        if (texturePath == "") {
            return new Mesh(vertices, indices);
        }
        return new Mesh(vertices, indices, texturePath);
    }
    inline Mesh* Sphere(float radius) {
        return Sphere("", 32, 16, radius);
    }
}

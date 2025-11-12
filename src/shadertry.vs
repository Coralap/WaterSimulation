#version 330 core

layout(location = 0) in vec3 aPos;       // Vertex position
layout(location = 1) in vec3 aNormal;    // Vertex normal
layout(location = 2) in vec2 aTexCoords; // Vertex texture coords

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    FragPos = aPos;    // just pass through the position
    Normal = aNormal;  // pass through normals
    TexCoords = aTexCoords; // pass through texture coordinates

    gl_Position = projection * view * model * vec4(aPos, 1.0); // no transformations yet
}

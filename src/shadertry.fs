#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;
uniform sampler2D Texture;
uniform vec3 objectColor; // default color

void main()
{


    FragColor = vec4(0.024f, 0.812f, 0.89f,1.0f);    
}

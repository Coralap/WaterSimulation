#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;
uniform sampler2D Texture;
uniform vec3 objectColor = vec3(1.0, 0.5, 0.31); // default color

void main()
{
    // Just render the object with a flat color
    FragColor = texture(Texture, TexCoords);
}

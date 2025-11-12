
#pragma once
#include "stb/stb_image.h"
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Texture{
    public:
        std::string path;
    
        Texture(std::string path):
        path(path){

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);


            


            GenTexture();
        }


        void use(){
            glBindTexture(GL_TEXTURE_2D, texture);
        }
    
        private:
            unsigned int texture;
            void GenTexture(){
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                // set the texture wrapping/filtering options (on the currently bound texture object)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // load and generate the texture
                int width, height, nrChannels;
                unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
                if (data)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    std::cout << "Failed to load texture" << std::endl;
                }
                stbi_image_free(data);
            }

};

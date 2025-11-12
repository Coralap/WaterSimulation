#pragma once
#include <string>
#include "Window.h"
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "../camera/Camera.h"
#include "../renderer/Entity.h"
#include "../renderer/Model.h"

#include "../renderer/Shader.h"
#include "../renderer/Prefabs.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

void processInput(GLFWwindow *window);
class Application {
public:
    unsigned int width;
    unsigned int height;
    std::string name;
    Window window;

    Shader* shader;
    std::vector<Entity*> Entities;  
    Texture* texture;

    Camera* camera;
    Application(unsigned int width, unsigned int height, const std::string name);
    void Run();
};

Application::Application(unsigned int width, unsigned int height, const std::string name)
    : width(width), height(height), name(name), window(width, height, name)
{   
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }    
    camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
    // Simple vertex and fragment shaders
    shader = new Shader("../src/shadertry.vs", "../src/shadertry.fs");

 


    

    Model* cubemodel = new Model();
    cubemodel->AddMesh(Prefabs::Cube("../src/container.jpg"));
    Model* pyramidModel = new Model();
    pyramidModel->AddMesh(Prefabs::Pyramid("../src/container.jpg"));
    Entity* entity1 = new Entity(cubemodel);
    Entity* entity2 = new Entity(pyramidModel);
    Entities.push_back(entity1);
    Entities.push_back(entity2);

}

void Application::Run() {
    // -----------------------------
    // Setup ImGui context
    // -----------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window.GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130"); // OpenGL 3.0+ compatible

    // Color variable controlled by GUI
    float clear_color[3] = {0.2f, 0.3f, 0.3f};

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    // Main loop
    while (!window.ShouldClose()) {
        processInput(window.GetNativeWindow());

        // Start ImGui frame

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        int index =0;
        for(Entity* entity : Entities){
            entity->transform.position.x=index*2;
            entity->transform.position.z=-5;
            shader->setMat4("projection",projection);
            shader->setMat4("view", camera->GetViewMatrix());
            entity->Draw(*shader);
            index ++;
        }


        // -----------------------------
        // Render ImGui
        // -----------------------------
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.SwapBuffers();
        window.PollEvents();
    }

    // -----------------------------
    // Cleanup
    // -----------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    for (Entity* entity : Entities)
        delete entity;
    delete shader;
    glfwTerminate();
}

void processInput(GLFWwindow *window) { 
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true); 

}
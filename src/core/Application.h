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


void processInput(GLFWwindow *window, Camera* camera, float deltaTime);
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
 

    Model* sphereModel = new Model();
    sphereModel->AddMesh(Prefabs::Sphere());

    // Example: create a grid of small spheres
    const int gridSize = 5;
    const float spacing = 1.0f;
    for (int x = 0; x < gridSize; ++x) {
        for (int y = 0; y < gridSize; ++y) {
            for (int z = 0; z < gridSize; ++z) {
                Entity* sphere = new Entity(sphereModel);
                sphere->transform.position = glm::vec3(
                    (x - gridSize/2.0f) * spacing,
                    (y - gridSize/2.0f) * spacing,
                    (z - gridSize/2.0f) * spacing-5
                );
                Entities.push_back(sphere);
            }
        }
    }



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


        glfwSetInputMode(window.GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window.GetNativeWindow(), camera);

    // Setup mouse callbacks
    glfwSetCursorPosCallback(window.GetNativeWindow(), [](GLFWwindow* window, double xpos, double ypos){
        static float lastX = 400.0f;
        static float lastY = 300.0f;
        static bool firstMouse = true;

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
        cam->ProcessMouseMovement(xoffset, yoffset);
    });

    glfwSetScrollCallback(window.GetNativeWindow(), [](GLFWwindow* window, double xoffset, double yoffset){
        Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
        cam->ProcessMouseScroll((float)yoffset);
    });
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    // Main loop
    while (!window.ShouldClose()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window.GetNativeWindow(), camera, deltaTime);

        // Start ImGui frame

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        for(Entity* entity : Entities){
            shader->setMat4("projection",projection);
            shader->setMat4("view", camera->GetViewMatrix());
            entity->Draw(*shader);
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



void processInput(GLFWwindow *window, Camera* camera, float deltaTime) {
    const float cameraSpeed = 2.5f * deltaTime; // adjust as needed

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->ProcessKeyboard(DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

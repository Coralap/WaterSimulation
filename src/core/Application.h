#pragma once
#include <string>
#include "Window.h"
#include <vector>
#include <random>

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
struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
};
bool pWasPressed = false;
bool playing = false;
std::vector<Particle> particles;
Shader* texturedShader;


const int boxSize = 10;

const int num_particles = 125;
const float particleRadius =0.5f;
const float particleMass =1;

bool checkParticleCollision(Particle* p1, Particle* p2);
void resolveCollision(Particle& a, Particle& b);
void processInput(GLFWwindow *window, Camera* camera, float deltaTime);
class Application {
public:
    unsigned int width;
    unsigned int height;
    std::string name;
    Window window;

    Shader* shader;
    std::vector<Entity*> Entities;  
    std::vector<Entity*> Objects;  

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
    texturedShader = new Shader("../src/shadertry.vs", "../src/texturedShader.fs");
    
    Model* boxModel = new Model();
    boxModel->AddMesh(Prefabs::Cube("../src/container.jpg"));

    Entity* floor = new Entity(boxModel);
    floor->transform.position = glm::vec3(0.0f, -boxSize/2, -5.0f);
    floor->transform.scale = glm::vec3(boxSize, 0.01f, boxSize);
    Objects.push_back(floor);


    particles.resize(num_particles);
    Model* sphereModel = new Model();
    sphereModel->AddMesh(Prefabs::Sphere(particleRadius));

    int cubeSize = std::cbrt(num_particles);
    int index = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> velDist(-1.0f, 1.0f);
    for (int x = 0; x < cubeSize; x++) {
        for (int y = 0; y < cubeSize; y++) {
            for (int z = 0; z < cubeSize; z++) {

                if (index >= num_particles) break;

                particles[index].position = glm::vec3(-boxSize/2+x, y, z - 5.0f);
                particles[index].velocity = glm::vec3(0.0f);

                Entity* sphere = new Entity(sphereModel);
                sphere->transform.position = particles[index].position;
                particles[index].velocity = glm::vec3(
                    velDist(rng),
                    velDist(rng),
                    velDist(rng)
                );
                Entities.push_back(sphere);
                index++;
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
        glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        if(playing)
        for (int i = 0; i < num_particles; i++) {
                Particle& p = particles[i];


                p.velocity += gravity * deltaTime;
                p.position += p.velocity * deltaTime;
                if(p.position.y-particleRadius<=-boxSize/2){
                    p.position.y =-boxSize/2 +particleRadius;
                    p.velocity.y = -p.velocity.y *0.9f; // damping
                }

                if(p.position.x-particleRadius<=-boxSize/2){
                    p.position.x =-boxSize/2 +particleRadius;
                    p.velocity.x = -p.velocity.x *0.9f; // damping
                }
                if(p.position.x+particleRadius>=boxSize/2){
                    p.position.x =boxSize/2 -particleRadius;
                    p.velocity.x = -p.velocity.x *0.9f; // damping
                }

                if(5+p.position.z-particleRadius<=-boxSize/2){
                    p.position.z =-boxSize/2 +particleRadius-5;
                    p.velocity.z = -p.velocity.z *0.9f; // damping
                }
                if(5+p.position.z+particleRadius>=boxSize/2){
                    p.position.z =boxSize/2 -particleRadius-5;
                    p.velocity.z = -p.velocity.z *0.9f; // damping
                }




                for(int j =0; j<num_particles;j++){
                    if(i!=j&& checkParticleCollision(&p,&particles[j])){
                        resolveCollision(p,particles[j]);
                    }

                }
                
                Entities[i]->transform.position = p.position;
        }








        // Start ImGui frame

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader->use();
        shader->setMat4("projection",projection);
        shader->setMat4("view", camera->GetViewMatrix());   
        for(Entity* entity : Entities){

            entity->Draw(*shader);
        }

        texturedShader->use();
        texturedShader->setMat4("projection",projection);
        texturedShader->setMat4("view", camera->GetViewMatrix());
        texturedShader->setInt("Texture", 0);
        for(Entity* entity : Objects){
            entity->model->meshes[0]->texture->use();

            entity->Draw(*texturedShader);
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

bool checkParticleCollision(Particle* p1, Particle* p2){
    glm::vec3 normal = p1->position - p2->position;
    float dist = glm::length(normal);
    return (dist < 2*particleRadius);

}

void resolveCollision(Particle& a, Particle& b)
{
    glm::vec3 normal = b.position - a.position;
    float dist = glm::length(normal);
    if (dist == 0.0f) return;

    // Normalize
    glm::vec3 n = normal / dist;

    float overlap = (particleRadius*2) - dist;

    // Push each particle away half the penetration distance
    a.position -= n * (overlap * 0.5f);
    b.position += n * (overlap * 0.5f);

        glm::vec3 relVel = b.velocity - a.velocity;

    // Relative velocity along normal
    float velAlongNormal = glm::dot(relVel, n);

    // If particles are separating, no collision
    if (velAlongNormal > 0)
        return;

    float restitution = 0.9f; // bounciness

    // Impulse scalar
    float j = -(1 + restitution) * velAlongNormal;
    j /= (1 / particleMass + 1 / particleMass);

    // Apply impulses
    glm::vec3 impulse = j * n;

    a.velocity -= impulse / particleMass;
    b.velocity += impulse / particleMass;
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

    bool pIsPressed = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;

    if (pIsPressed && !pWasPressed) {
        playing = !playing;
    }

    pWasPressed = pIsPressed; // update previous state

}

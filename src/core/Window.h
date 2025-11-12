#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Window {
public:
    unsigned int width;
    unsigned int height;
    std::string name;
    GLFWwindow* raw_window;

    Window(unsigned int width, unsigned int height, std::string name) {
        this->width = width;
        this->height = height;
        this->name = name;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        raw_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
        if (raw_window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        glfwMakeContextCurrent(raw_window);
        glfwSetFramebufferSizeCallback(raw_window, framebuffer_size_callback);
    }

    ~Window() {
        glfwDestroyWindow(raw_window);
    }

    bool ShouldClose() const { return glfwWindowShouldClose(raw_window); }
    void PollEvents() const { glfwPollEvents(); }
    void SwapBuffers() const { glfwSwapBuffers(raw_window); }


    GLFWwindow* GetNativeWindow() const { return raw_window; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    void SetName(const std::string& name) { glfwSetWindowTitle(raw_window, name.c_str()); }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

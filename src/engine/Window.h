#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Window {
public:
    unsigned int height = 480, width = 854;

    void init();

    void swapBuffers();
    void pollEvents();

    GLFWwindow* getWindow() const {
        return window;
    }

    bool windowClosed() {
        return glfwWindowShouldClose(window);
    }
private:
    GLFWwindow* window = nullptr;
};
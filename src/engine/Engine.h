#pragma once

#include <functional>

#include "Window.h"
#include "InputManager.h"

class Engine {
public:
    Engine();

    void init();
    void run();
    void shutdown();

    std::function<void()> onInit;
    std::function<void(float dt)> onFixedUpdate;
    std::function<void()> onUpdate;
    std::function<void(float dt, float elapsed)> onRender;
    std::function<void()> onShutdown;

    GLFWwindow* getWindow() const {
        return window.getWindow();
    }

    Window& getWindowClass() {
        return window;
    }

    InputManager& getInputManager() {
        return inputManager;
    }

private:
    unsigned int screenHeight = 480, screenWidth = 854;

    Window window;
    InputManager inputManager;
};
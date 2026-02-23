#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <utility>


class InputManager {
public:
    void init(GLFWwindow* w, unsigned int screenHeight, unsigned int screenWidth);
    void update();

    bool isKeyDown(int key) const;
    bool isKeyPressed(int key) const;

    bool isMouse1Down();
    bool isMouse2Down();
    bool isMouse1Pressed();
    bool isMouse2Pressed();

    void centerCursorPos();

    std::pair<float, float> mouseDelta() const;

    GLFWwindow* getWindow();

private:
    GLFWwindow* window;

    unsigned int screenHeight, screenWidth;

    static constexpr int MAX_KEY = 512;
    bool currKeys[MAX_KEY]{};
    bool prevKeys[MAX_KEY]{};

    bool currMouse1 = false;
    bool currMouse2 = false;
    bool prevMouse1 = false;
    bool prevMouse2 = false;
};
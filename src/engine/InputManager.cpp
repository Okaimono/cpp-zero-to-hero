#include "InputManager.h"

#include <cstring>
#include <cassert>

void InputManager::init(GLFWwindow* w, unsigned int sh, unsigned int sw)
{
    window = w;
    screenHeight = sh;
    screenWidth = sw;
}

void InputManager::update()
{
    prevMouse1 = currMouse1;
    prevMouse2 = currMouse2;
    currMouse1 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    currMouse2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    std::memcpy(prevKeys, currKeys, sizeof(currKeys));

    for (int key = 0; key < MAX_KEY; key++) {
        currKeys[key] = glfwGetKey(window, key) == GLFW_PRESS;
    }
}

std::pair<float, float> InputManager::mouseDelta() const {
    double mouseX;
    double mouseY;

    glfwGetCursorPos(window, &mouseX, &mouseY);

    float mouseDeltaX = (float)((mouseY - (screenHeight / 2)) / screenHeight);
    float mouseDeltaY = (float)((mouseX - (screenWidth / 2)) / screenWidth);

    return {mouseDeltaX, mouseDeltaY};
}

void InputManager::centerCursorPos()
{
    glfwSetCursorPos(window, (screenWidth / 2), (screenHeight / 2));
}

GLFWwindow* InputManager::getWindow() {
    return window;
}

bool InputManager::isMouse1Down() {
    return currMouse1;
}

bool InputManager::isMouse2Down() {
    return currMouse2;
}

bool InputManager::isMouse1Pressed() {
    return currMouse1 && !prevMouse1;
}

bool InputManager::isMouse2Pressed() {
    return currMouse2 && !prevMouse2;
}

bool InputManager::isKeyDown(int key) const {
    return currKeys[key];
}

bool InputManager::isKeyPressed(int key) const {
    return currKeys[key] && !prevKeys[key];
}
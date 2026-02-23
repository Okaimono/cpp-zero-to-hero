#pragma once

#include "Camera.h"
#include "engine/InputManager.h"

struct PlayerActions {
    glm::vec2 lookDelta = {0.0f, 0.0f};

    int hotbarSlot = -1;

    bool eKeyPressed = false;
    bool cKeyPressed = false;
    bool pKeyPressed = false;
    bool escKeyPressed = false;

    bool forward, backward, left, right;
    bool sprint = false;
    bool jump = false;
    bool godJump = false;

    bool mouse1Down = false;
    bool mouse1Pressed = false;
    bool mouse2Down = false;
    bool mouse2Pressed = false;
};

class PlayerInput {
public:
    Camera& camera;
    InputManager& inputManager;

    PlayerInput(Camera& cam, InputManager& input, unsigned int sh, unsigned int sw);

    GLFWwindow* getWindow();

    PlayerActions poll();
private:
    const unsigned int screenHeight;
    const unsigned int screenWidth;

    void updateMouseInputs(PlayerActions& action);
    void updateKeyInputs(PlayerActions& action);
};
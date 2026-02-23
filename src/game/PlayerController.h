#pragma once

#include <glm/glm.hpp>

#include "game/actions.h"
#include "game/PlayerInput.h"
#include "Camera.h"

class PlayerController {
public:
    PlayerController(Camera& cam, PlayerInput& i)
        : camera(cam), input(i)
    {}
    void update(unsigned int screenHeight, unsigned int screenWidth);
private:
    Camera& camera;
    PlayerInput& input;

    void handleLook(const PlayerActions& action);
    void handleMovement(const PlayerActions& action);
    void handleMouseInputs(const PlayerActions& action, unsigned int screenHeight, unsigned int screenWidth);
    void handleKeyInputs(const PlayerActions& action);
};
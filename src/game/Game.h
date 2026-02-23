#pragma once

#include "shaderClass.h"
#include "Camera.h"
#include "game/PlayerInput.h"
#include "game/PlayerController.h"
#include "ArmHUD.h"
#include "engine/Engine.h"
#include "World.h"

class Game {
public:
    Game(Engine& e);
    void init();

    Engine& engine;

    std::unique_ptr<Shader> shader = nullptr;
	std::unique_ptr<World> world = nullptr;
	std::unique_ptr<Camera> camera = nullptr;
    //std::unique_ptr<PlayerCamera> playerCamera = nullptr;
	std::unique_ptr<PlayerInput> playerInput = nullptr;
	std::unique_ptr<ArmHUD> arm = nullptr;
	std::unique_ptr<Texture> gameAtlas = nullptr;
    std::unique_ptr<PlayerController> playerController = nullptr;

private:
    const unsigned int screenHeight = 480;
    const unsigned int screenWidth = 854;

    // Variable for render distance (in chunks)
    const int renderDistRadius = 45;
    const float FOV = 100.0f;

};
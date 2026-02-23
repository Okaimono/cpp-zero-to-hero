#include "game/Game.h"

#include "World.h"

namespace fs = std::filesystem;

Game::Game(Engine& e)
    : engine(e)
{}

void Game::init()
{
    engine.onInit = [&]() {
		engine.init();
		shader = std::make_unique<Shader>("ResourceFiles/Shaders/default.vert", 
			"ResourceFiles/Shaders/fragment.frag");
		std::string parentDir = fs::current_path().string();
		std::string texPath = "/ResourceFiles/Texture/";
		gameAtlas = std::make_unique<Texture>((parentDir + texPath + "game_atlas.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		gameAtlas->texUnit(*shader, "tex0", 0);

		world = std::make_unique<World>(renderDistRadius);
		world->initializeWorld();
		PhysicsEngine::setWorld(&*world);
		camera = std::make_unique<Camera>(screenWidth, screenHeight, glm::vec3(0.0f, 100.0f, 0.0f), world.get(), *shader);
		arm = std::make_unique<ArmHUD>(*shader, *camera);
		playerInput = std::make_unique<PlayerInput>(*camera, engine.getInputManager(), screenHeight, screenWidth);
		playerController = std::make_unique<PlayerController>(*camera, *playerInput);
	};

    engine.onUpdate = [&]() {
		Window& window = engine.getWindowClass();
		playerController->update(window.height, window.width);
	};

	engine.onFixedUpdate = [&](float dt) {
		camera->update(engine.getWindow());
		camera->cameraTick(dt);
		world->gameTick(dt);
		PhysicsEngine::updateWorldPhysics(dt);
	};

	engine.onRender = [&](float dt, float elapsed) {
		glClearColor(120.0f, 167.0f, 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		shader->Activate();
		gameAtlas->Bind();
		camera->Matrix(FOV, 0.1f, 20000.0f, "camMatrix");
		world->renderWorld(camera->getCamMatrix());

		glClear(GL_DEPTH_BUFFER_BIT);

		arm->renderHand(*camera, elapsed, dt);
		camera->renderGUI();
	};

	engine.run();
}
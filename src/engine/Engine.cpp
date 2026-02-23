#include "Engine.h"

#include <iostream>
#include <chrono>


Engine::Engine() {}

void Engine::init()
{
	window.init();
	inputManager.init(window.getWindow(), screenHeight, screenWidth);

}

void Engine::run() 
{
	onInit();
	
	const float FIXED_DT = 1.0f / 120.0f;
	auto lastPhysicsUpdate = std::chrono::high_resolution_clock::now();

	while (!window.windowClosed()) {
		auto now = std::chrono::high_resolution_clock::now();
		float elapsed = std::chrono::duration<float>(now - lastPhysicsUpdate).count();
		if (elapsed >= FIXED_DT) {
			onFixedUpdate(FIXED_DT);
			lastPhysicsUpdate = now;
		}
		onUpdate();
		inputManager.update();
		onRender(FIXED_DT, elapsed);

		window.swapBuffers();
		window.pollEvents();
	}
}
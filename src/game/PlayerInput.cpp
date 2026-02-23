#include "game/PlayerInput.h"

PlayerInput::PlayerInput(Camera& cam, InputManager& input, unsigned int sh, unsigned int sw)
    : camera(cam), inputManager(input), screenHeight(sh), screenWidth(sw)
{}

PlayerActions PlayerInput::poll()
{
	PlayerActions action{};

	updateKeyInputs(action);
	updateMouseInputs(action);

	return action;
}

void PlayerInput::updateKeyInputs(PlayerActions& action)
{
	action.eKeyPressed = inputManager.isKeyPressed(GLFW_KEY_E);
	action.cKeyPressed = inputManager.isKeyPressed(GLFW_KEY_C);
	action.pKeyPressed = inputManager.isKeyPressed(GLFW_KEY_P);
	action.escKeyPressed = inputManager.isKeyPressed(GLFW_KEY_ESCAPE);

	action.forward = inputManager.isKeyDown(GLFW_KEY_W);
	action.backward = inputManager.isKeyDown(GLFW_KEY_S);
	action.left = inputManager.isKeyDown(GLFW_KEY_A);
	action.right = inputManager.isKeyDown(GLFW_KEY_D);

	action.sprint = inputManager.isKeyDown(GLFW_KEY_LEFT_SHIFT);
	action.jump = inputManager.isKeyPressed(GLFW_KEY_SPACE);
	action.godJump = inputManager.isKeyDown(GLFW_KEY_SPACE);

	if (inputManager.isKeyPressed(GLFW_KEY_1)) action.hotbarSlot = 0;
	else if (inputManager.isKeyPressed(GLFW_KEY_2)) action.hotbarSlot = 1;
	else if (inputManager.isKeyPressed(GLFW_KEY_3)) action.hotbarSlot = 2;
	else if (inputManager.isKeyPressed(GLFW_KEY_4)) action.hotbarSlot = 3;
	else if (inputManager.isKeyPressed(GLFW_KEY_5)) action.hotbarSlot = 4;
	else if (inputManager.isKeyPressed(GLFW_KEY_6)) action.hotbarSlot = 5;
	else if (inputManager.isKeyPressed(GLFW_KEY_7)) action.hotbarSlot = 6;
	else if (inputManager.isKeyPressed(GLFW_KEY_8)) action.hotbarSlot = 7;
	else if (inputManager.isKeyPressed(GLFW_KEY_9)) action.hotbarSlot = 8;
	else action.hotbarSlot = -1;
}

void PlayerInput::updateMouseInputs(PlayerActions& action)
{
	if (camera.camLock) {
		std::pair<float, float> mouseDelta = inputManager.mouseDelta();
		action.lookDelta.x = mouseDelta.first;
		action.lookDelta.y = mouseDelta.second;

		inputManager.centerCursorPos();
	}

	action.mouse1Down = inputManager.isMouse1Down();
	action.mouse1Pressed = inputManager.isMouse1Pressed();

	action.mouse2Down = inputManager.isMouse2Down();
	action.mouse2Pressed = inputManager.isMouse2Pressed();
}

GLFWwindow* PlayerInput::getWindow()
{
	return inputManager.getWindow();
}
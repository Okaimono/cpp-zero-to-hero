#include "game/PlayerController.h"
#include "Camera.h"

#include <iostream>

void PlayerController::update(unsigned int screenHeight, unsigned int screenWidth)
{
    PlayerActions action = input.poll();

    handleLook(action);
    handleMovement(action);
    handleMouseInputs(action, screenHeight, screenWidth);
	handleKeyInputs(action);
}

void PlayerController::handleLook(const PlayerActions& action)
{
    auto mouseDelta = action.lookDelta;

    float rotX = camera.sensitivity * mouseDelta.x;
    float rotY = camera.sensitivity * mouseDelta.y;

    glm::vec3 newOrientation = glm::rotate(camera.Orientation, glm::radians(-rotX), glm::normalize(glm::cross(camera.Orientation, camera.Up)));

    camera.rightPlane = glm::normalize(glm::rotate(camera.Orientation, glm::radians(-100.0f / 2.0f), camera.Up));
    camera.leftPlane = glm::normalize(glm::rotate(camera.Orientation, glm::radians(100.0f / 2.0f), camera.Up));

    if (!(glm::angle(newOrientation, camera.Up) <= glm::radians(5.0f) or glm::angle(newOrientation, -camera.Up) <= glm::radians(5.0f))) 
    {
        camera.Orientation = newOrientation;
    }

    camera.Orientation = glm::rotate(camera.Orientation, glm::radians(-rotY), camera.Up);
}

void PlayerController::handleMovement(const PlayerActions& action)
{
	if (action.sprint) {
		camera.sprinting = true;
		camera.speed = 5.612f;
		if (camera.godMode) {
			camera.speed = 150.0f;
		}
	}
	else {
		camera.sprinting = false;
		camera.speed = 4.317f;
	}

    camera.forward  = action.forward;
    camera.backward  = action.backward;
    camera.left  = action.left;
    camera.right  = action.right;

    camera.walk();

	camera.isOnGround = camera.playerPhysics.isOnGround();
	bool keySpaceDown = action.jump && camera.isOnGround;
	if (!camera.godMode && keySpaceDown) {
		camera.jump();
	} 
	else if (camera.godMode && action.godJump) {
		camera.jump();
	}
}

void PlayerController::handleMouseInputs(const PlayerActions& action, unsigned int screenHeight, unsigned int screenWidth)
{
	// MOUSE CLICK
	if ((action.mouse1Down || action.mouse2Down) && *camera.guiState == GUIState::HUD) {
		camera.mineBlock = true;
		camera.swingArm = true;
	}
	else if (!(action.mouse1Down && action.mouse2Down) && *camera.guiState == GUIState::HUD) {
		camera.blockInteraction.resetBlockStatus = true;
		camera.swingArm = false;
		camera.mineBlock = false;
	}

	if (action.mouse1Pressed && *camera.guiState != GUIState::HUD) {
		double mouseX, mouseY;
		GLFWwindow* window = input.getWindow();
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float glX = mouseX;
		float glY = screenHeight - mouseY;

		camera.playerGui->playerClick(glX, glY, true);
		camera.playerGui->refreshHotbar();
		camera.playerGui->refreshAllInventoryUI();
		camera.playerInventory.updateHeldItemDisplay();
	}

	if (action.mouse2Pressed) {
		if (*camera.guiState == GUIState::HUD) {
			if (!camera.swingArm) {
				camera.swingArm = true;
			} else {
				camera.swingArm = false;
			}
			ItemID checkIfInteract = camera.blockInteraction.Interact(camera.Orientation, camera.Position, camera.world, camera.playerGui);

			if (checkIfInteract == ItemID::NONE) { } 
			else if (checkIfInteract == ItemID::CRAFTINGTABLE_BLOCK) {
				camera.playerGui->changeGUIState(GUIState::CRAFTING_TABLE);
				camera.swingArm = false;
				camera.mineBlock = false;
			}
			else if (checkIfInteract == ItemID::BARREL_BLOCK) {
				camera.playerGui->changeGUIState(GUIState::BARREL);
				camera.swingArm = false;
				camera.mineBlock = false;
			}
			else if (checkIfInteract == ItemID::FURNACE_BLOCK) {
				camera.playerGui->changeGUIState(GUIState::FURNACE);
				camera.mineBlock = false;
			}
		}
		else if (*camera.guiState == GUIState::INVENTORY || *camera.guiState == GUIState::CRAFTING_TABLE || *camera.guiState == GUIState::BARREL || *camera.guiState == GUIState::FURNACE) {
			double mouseX, mouseY;
			glfwGetCursorPos(input.getWindow(), &mouseX, &mouseY);
			
			float glX = mouseX;
			float glY = screenHeight - mouseY;
			camera.playerGui->playerClick(glX, glY, false);
		}
		camera.playerGui->refreshAllInventoryUI();
		camera.playerInventory.updateHeldItemDisplay();
	}
}

void PlayerController::handleKeyInputs(const PlayerActions& action)
{
	const int& hotbarSlot = action.hotbarSlot;
	const bool& eKeyPressed = action.eKeyPressed;
	const bool& cKeyPressed = action.cKeyPressed;
	const bool& pKeyPressed = action.pKeyPressed;
	const bool& escKeyPressed = action.escKeyPressed;

	camera.changeHotbar(hotbarSlot);

	if (cKeyPressed) {
		glm::vec3 Position = camera.getPosition();
		std::cout << "x: " << round(Position.x * 100.0f) / 100.0f << ", y: " << round(Position.y * 100.0f) / 100.0f << ", z: " << round(Position.z * 100.0f) / 100.0f << std::endl;
	}
	if (pKeyPressed) {
		camera.godMode = !camera.godMode;
	}
	if (eKeyPressed) {
		camera.eKeyState = !camera.eKeyState;
		if (camera.eKeyState) {
			camera.mineBlock = false;
			camera.swingArm = false;
			camera.playerGui->changeGUIState(GUIState::INVENTORY);
			camera.playerGui->refreshInventoryInterface(0.0f, 0.0f);
		}
		else {
			camera.playerGui->refreshInventoryInterface(0.0f, 0.0f);
			camera.playerGui->changeGUIState(GUIState::HUD);
		}
		camera.playerGui->refreshHotbar();
	}
	if (escKeyPressed) {
		if (*(camera.guiState) == GUIState::HUD) {
			camera.playerGui->changeGUIState(GUIState::MENU);
		}
		else if (*(camera.guiState) == GUIState::CRAFTING_TABLE) {
			camera.playerGui->changeGUIState(GUIState::HUD);
		}
		else if (*(camera.guiState) == GUIState::BARREL) {
			camera.playerGui->changeGUIState(GUIState::HUD);
		}
		else if (*(camera.guiState) == GUIState::FURNACE) {
			camera.playerGui->changeGUIState(GUIState::HUD);
		}
	}
}
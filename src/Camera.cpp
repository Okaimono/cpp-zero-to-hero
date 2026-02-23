#include"Camera.h"

#include "engine/InputManager.h"
#include <utility>


Camera::Camera(unsigned int sw, unsigned int sh, glm::vec3 position, World* w, Shader& s) 
	: screenWidth(sw),
	screenHeight(sh),
	shader(s),
	Position(position), 
	world(w), 
	playerPhysics(Position, Orientation, Velocity), 
	blockInteraction(&playerInventory)
{
	itemSelected = new bool(false);
	playerGui = new GUIManager(sw, sh, playerInventory, itemSelected);
	guiState = &(playerGui->guiState);

	initializeEntityManager();
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, const char* uniform)
{
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)screenWidth / screenHeight, nearPlane, farPlane);
	camMatrix = projection * view;

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(camMatrix));
}

void Camera::renderGUI()
{
	playerGui->RenderGUI();
}

Shader* Camera::getShaderPointer()
{
	return &shader;
}

void Camera::initializeEntityManager()
{
	world->initializeEntityManager(this);
}

glm::mat4 Camera::getViewMatrix()
{
	return view;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projection;
}

glm::vec3 Camera::getOrientation()
{
	return Orientation;
}

glm::vec3 Camera::getPosition()
{
	return Position;
}

glm::vec3& Camera::getPositionRef()
{
	return Position;
}

PlayerInventory& Camera::getPlayerInventory()
{
	return playerInventory;
}

void Camera::updateCursorLock(GLFWwindow* window)
{
    bool shouldLock = (*guiState == GUIState::HUD);
    static bool currentlyLocked = false;

    if (shouldLock != currentlyLocked)
    {
        if (shouldLock)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // Reset cursor position so first movement isn't huge jump
            glfwSetCursorPos(window, screenWidth / 2, screenHeight / 2);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPos(window, screenWidth / 2, screenHeight / 2);
        }
        currentlyLocked = shouldLock;
    }

    camLock = shouldLock; // keep this synced with rest of code
}

void Camera::explosion(glm::vec3 center, float radius)
{
	world->explodeAtCoords(center, radius);
}

void Camera::update(GLFWwindow* window)
{
	updateCursorLock(window);
	setUserChunkPosition();

	if (*itemSelected) {
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		
		float glX = mouseX;
		float glY = screenHeight - mouseY;

		if (*guiState ==  GUIState::INVENTORY)
			playerGui->refreshInventoryInterface(glX, glY);
		else if (*guiState == GUIState::CRAFTING_TABLE)
			playerGui->refreshCraftingTableInterface(glX, glY);
		else if (*guiState == GUIState::BARREL)
			playerGui->refreshChestInterface(glX, glY);
		else if (*guiState == GUIState::FURNACE)
			playerGui->refreshFurnaceInterface(glX, glY);
	}
}

void Camera::cameraTick(const float dt) {
	if (mineBlock) {
		if (blockInteraction.mineBlock(Orientation, Position, world, mineSpeed, dt)) {
			playerGui->refreshHotbar();
		}
	}
	playerGui->updateSmeltingInterface();
}

void Camera::setUserChunkPosition()
{
	if ((int) floor( Position.x / 16) != world->userChunkPosition.first || (int) floor(Position.z / 16) != world->userChunkPosition.second ) {
		world->userChunkPosition = {  (int) floor(Position.x / 16), (int) floor(Position.z / 16) };
		world->resetRenderArea();
	}
}

void Camera::changeHotbar(int key)
{
	if (key != -1) {
		playerInventory.changeItemSlot(key);
		playerGui->refreshHotbar();
	}
}

// Checks if player is walking, gets direction
bool Camera::isWalking()
{
	return walking;
}

bool Camera::isSprinting()
{
	return sprinting;
}

bool Camera::isArmSwinging()
{
	return swingArm;
}

// WILL REFRACTOR. Temporary rough draft.
void Camera::walk() {
	walking = forward || right || backward || left;
	if (!walking) {
		return;
	}
    glm::vec2 direction = {0, 0};
    
    if (forward) direction += glm::vec2{Orientation.x, Orientation.z};
    if (right) direction += glm::vec2{-Orientation.z, Orientation.x};
    if (backward) direction += glm::vec2{-Orientation.x, -Orientation.z};
    if (left) direction += glm::vec2{Orientation.z, -Orientation.x};

    float len = glm::length(direction);
    if (len < 0.001f) {
        return;  // Don't update velocity if not moving
    }

    direction = glm::normalize(direction) * speed;
    Velocity = {direction.x, Velocity.y, direction.y};
}

// Applys vector to jump
void Camera::jump() {
// 	// keeping it here for performance, will refractor later
	float jumpSpeed = 8.0f; // ACTUAL JUMP SPEED DONT REPLACE
	if (godMode) {
		jumpSpeed = 0.3f;
	}
	Velocity.y += jumpSpeed;
}
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "World.h"
#include "shaderClass.h"
#include "GUIManager.h"
#include "BlockInteraction.h"
#include "PhysicsEngine.h"
#include "PlayerInventory.h"

class InputManager;

class Camera 
{
    public:
        Camera(unsigned int sw, unsigned int sh, glm::vec3 position, World* w, Shader& s);

        PlayerInventory playerInventory;

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        void Matrix(float FOVdeg, float nearPlane, float farPlane, const char* uniform);
        void update(GLFWwindow* window);
        void printLocation();
        void cameraTick(float dt);
        void renderGUI();
        void renderInventory();
        void updateBindedChest();
        void explosion(glm::vec3 center, float radius);
        void updateHeldItemDisplay();

        void update(InputManager& inputManager, unsigned int screenHeight, unsigned int screenWidth);
        void updateMovementInputs(InputManager& inputManager);
        
        PlayerInventory& getPlayerInventory();
        Shader* getShaderPointer();
        // Get view and projection matrices for sky rendering
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();

        glm::vec3 getOrientation();
        glm::vec3 getPosition();
        glm::vec3 getUp();

        glm::vec3& getPositionRef();

        void initializeEntityManager();
        bool isArmSwinging();
        bool isWalking();
        bool isSprinting();

        const glm::mat4& getCamMatrix() { return camMatrix; }

        bool isOnGround = false;
        
        bool* itemSelected = nullptr;

        glm::vec3 Position;
        glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 Velocity = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 camMatrix;

        glm::vec3 rightPlane;
        glm::vec3 leftPlane;

        Shader& shader;
        World* world;
        GUIManager* playerGui;
        GUIState* guiState = nullptr;
        
        PhysicsEngine playerPhysics;
        BlockInteraction blockInteraction;

        unsigned int screenWidth;
        unsigned int screenHeight;
        
        bool mineBlock = false;
        bool swingArm = false;

        float mineSpeed = 5.0f;

        float speed = 4.317f;
        float sensitivity = 100.0f;

        bool walking = false;
        bool sprinting = false;
        bool forward = false;
        bool right = false;
        bool backward = false;
        bool left = false;

        bool eKeyState = false;

        bool camLock = true;

        bool godMode = false;

        const float FOV = 100.0f;

        void changeHotbar(int key);

        void walk();
        void jump();
    private:
        void updateCursorLock(GLFWwindow* window);
        void setUserChunkPosition();
};

#endif
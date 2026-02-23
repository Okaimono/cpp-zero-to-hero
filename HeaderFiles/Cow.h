#define GLM_ENABLE_EXPERIMENTAL
#ifndef COW_H
#define COW_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Mob.h"

#include <filesystem>
#include <deque>
#include <queue>
#include <unordered_set>
#include <vector>
#include <string>
#include <cmath>
#include <memory>

#include "PhysicsEngine.h" 

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

class Cow : public Mob {
    public:
        VAO headVAO, bodyVAO, legVAO;
        VBO headVBO, bodyVBO, legVBO;
        EBO headEBO, bodyEBO, legEBO;

        Texture texture;

        Shader* shader = nullptr;
        Camera* camera = nullptr;

        size_t headIndicesSize;
        size_t bodyIndicesSize;
        size_t legIndicesSize;

        glm::vec3 Position;
        glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);

        glm::vec3 headOrientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 realHeadOrientation = glm::vec3(0.0f, 0.0f, -1.0f);

        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 Velocity = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3& playerPosRef;

        std::unique_ptr<PhysicsEngine> creeperPhysics;

        Cow(Shader* s, Camera* cam, glm::vec3 spawnPos);
        ~Cow();

        void gameTick(World* world);
        void getOrientation();
        void renderCow();
        void renderLeg(const glm::mat4& camMatrix, const glm::vec3& legPos, const glm::mat4& orientParts, float swingAngle);
        void matrix();
        void renderHead();
        void explosion();
        void createHead();
        void createBody();
        void createLeg();
        bool destroyCreeper();

        std::vector<glm::vec3> getVertices(float sideHeight, float sideWidth, float sideDepth, FaceDirection dir);
        std::vector<glm::vec2> getUVCoords(float topLeftX, float topLeftY, float faceHeight, float faceWidth, float texWidth, float texHeight);

    private:
        static constexpr float WALK_SPEED = 4.0f;
        static constexpr float dt         = 1.0f / 120.0f;   // ← fixed!

        inline static const float accel = [](){
            constexpr float friction    = 0.85f;
            constexpr float targetSpeed = WALK_SPEED;
            constexpr float rampTime    = 0.35f;
            float k = std::log(friction) / dt;
            return (-k * targetSpeed) / (1.0f - std::exp(rampTime * k));
        }();   // ← these () are required!

        std::deque<Node> pathToFollow;

        glm::vec3 lastPathTarget = glm::vec3(0);    
        const float MIN_UPDATE_TIME = 1.0f;     // Min 0.5s between updates
        const float MIN_DISTANCE_MOVED = 3.0f;  // Or player moved 3+ block
        float pathUpdateTimer = 0.0f;

        float mobAngleX = 0.0f;
        float theta = 0.0f;
        float countdown = 1.5f;

        float HP;
        float maxHP;
        
        bool walking = true;
        bool destroy = false;
        bool showDebug = false;

        float countdownRadius = 3.0f;
        size_t radius = 4;

        void getNextNode();
        void createPath(World* world);
        void aStar(World* world);
        float octileDistance(glm::vec3 a, glm::vec3 b);
        void renderDebugCube();  
        void getHeadOrientation();

        void jump();
};

#endif
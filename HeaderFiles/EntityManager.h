#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "glm/glm.hpp"

#include "Creeper.h"
#include "Cow.h"
#include <memory>

class Camera;

class EntityManager
{
    public:
        EntityManager();

        void initializeEntityManager(Camera* cam);
        void debugCowTick(float dt, World* world);
        void generateCow(glm::vec3 coords);
        void gameMobTick(float dt, World* world);
        void renderMobs();
        void generateMob(glm::vec3 coords);

        void debug();

    private:
        Texture creeperTex;
        Shader* shader;
        glm::mat4* camMat;
        Camera* camera;

        std::vector<std::unique_ptr<Creeper>> creepers;

        std::unique_ptr<Cow> testCow;
        std::unique_ptr<Creeper> testCreeper;

        bool debugCow = false;
        bool debugCreeper = false;
};

#endif
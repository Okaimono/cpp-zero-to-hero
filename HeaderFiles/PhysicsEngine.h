#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <glm/glm.hpp>
#include <vector>


class World;

class PhysicsEngine {
    public:
        PhysicsEngine(glm::vec3& pos, glm::vec3& ori, glm::vec3& vel, float h = 1.6f);

        static void removeEngine(int ID);
        static void updateWorldPhysics(float PHYSICS_DT);
        static void setWorld(World* w);

        int getID();
        void updatePhysics(float dt);
        bool isOnGround();

    private:
        static inline World* world = nullptr;
        static std::vector<PhysicsEngine*> engines;
        static inline int nextID = 1;
        int ID = 0;

        const float height = 1.6;

        glm::vec3& Position;
        glm::vec3& Orientation;
        glm::vec3& Velocity;

        const glm::vec3 GRAVITY_CONSTANT = {0, -32.0f, 0};
        const float speed = 5.0f;

        bool checkBlockClip(glm::vec3 playerHitBox);
};

#endif
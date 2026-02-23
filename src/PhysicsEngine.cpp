#include "PhysicsEngine.h"
#include "Chunk.h"
#include <cmath>
#include "World.h"

// REFRACTOR: Tryin

std::vector<PhysicsEngine*> PhysicsEngine::engines;

PhysicsEngine::PhysicsEngine(glm::vec3& pos, glm::vec3& ori, glm::vec3& vel, float h)
    : Position(pos), Orientation(ori), Velocity(vel), height(h)
{
    engines.push_back(this);
    ID = nextID;
    nextID++;
}

int PhysicsEngine::getID()
{
    return ID;
}

void PhysicsEngine::removeEngine(int ID)
{
    for (auto it = engines.begin(); it != engines.end(); ++it) {
        if ((*it)->getID() == ID) {
            engines.erase(it);
            return;
        }
    }
}

void PhysicsEngine::setWorld(World* w)
{
    world = w;
}

void PhysicsEngine::updatePhysics(const float PHYSICS_DT) {

    Position += Velocity * PHYSICS_DT;

    const float FRICTION_CONST = 0.85f;
    const float FRICTION_MULTIPLIER = FRICTION_CONST;

    Velocity.x *= FRICTION_MULTIPLIER;
    Velocity.z *= FRICTION_MULTIPLIER;

    glm::vec3 playerHitBoxDown = Position + glm::vec3{0, -(height + 0.01f), 0};
    ItemID* blockClipDown = world->getBlockAtCoords(playerHitBoxDown);

    if (blockClipDown == nullptr || !isOnGround())
    {
        Velocity += GRAVITY_CONSTANT * PHYSICS_DT;
    } 
    else 
    {
        if (Velocity.y < 0.0f)
        {
            Position.y = floorf(Position.y) + (height - 1.0f);
            Velocity.y = 0.0f;
        }
    }

    // Forward
    glm::vec3 playerClipForwardWest = glm::vec3(-0.24f, 0, -0.3f);
    glm::vec3 playerClipForwardEast = glm::vec3(0.24f, 0, -0.3f);
    if ((checkBlockClip(playerClipForwardWest) || checkBlockClip(playerClipForwardEast)) && Velocity.z < 0) 
    {
        Position.z = floorf(Position.z) + 0.3f;
        Velocity.z = 0;
    }

    // Right
    glm::vec3 playerClipRightNorth = glm::vec3(0.3f, 0, -0.24f);
    glm::vec3 playerClipRightSouth = glm::vec3(0.3f, 0, 0.24f);
    if ((checkBlockClip(playerClipRightNorth) || checkBlockClip(playerClipRightSouth)) && Velocity.x > 0) 
    {
        Position.x = ceil(Position.x) - 0.3f;
        Velocity.x = 0;
    }

    // Backward
    glm::vec3 playerClipBackwardWest = glm::vec3(-0.24f, 0, 0.3f);
    glm::vec3 playerClipBackwardEast = glm::vec3(0.24f, 0, 0.3f);
    if ((checkBlockClip(playerClipBackwardWest) || checkBlockClip(playerClipBackwardEast)) && Velocity.z > 0)
    {
        Position.z = ceil(Position.z) - 0.3f;
        Velocity.z = 0;
    }

    // Left
    glm::vec3 playerClipLeftNorth = glm::vec3(-0.3f, 0, -0.24f);
    glm::vec3 playerClipLeftSouth = glm::vec3(-0.3f, 0, 0.24f);
    if ((checkBlockClip(playerClipLeftNorth) || checkBlockClip(playerClipLeftSouth)) && Velocity.x < 0)
    {
        Position.x = floorf(Position.x) + 0.3f;
        Velocity.x = 0;
    }
        
}

void PhysicsEngine::updateWorldPhysics(float PHYSICS_DT) {
    for (int i = 0; i < engines.size(); i++) {
        //std::cout << "Creeper " << i << ": " << engines[i]->Position.x << ", " << engines[i]->;
        engines[i]->updatePhysics(PHYSICS_DT);
    }
    // for (PhysicsEngine* engine : engines) {
    //     engine->updatePhysics(PHYSICS_DT);
    // }
}

bool PhysicsEngine::checkBlockClip(glm::vec3 playerHitBox) {
    for (int i = 0; i < 3; i++) {
        glm::vec3 checkBlockPos = playerHitBox + Position;
        checkBlockPos.y = checkBlockPos.y - (height / 3) * i ;

        ItemID* checkBlock = world->getBlockAtCoords(checkBlockPos);
        if (checkBlock != nullptr && *checkBlock != ItemID::NONE) {
            return true;
        }
    }
    return false;
}

bool PhysicsEngine::isOnGround() {
    glm::vec3 playerHitBoxDown = Position + glm::vec3{0, -(height + 0.01f), 0};
    ItemID* blockClipDown = world->getBlockAtCoords(playerHitBoxDown);
    return (blockClipDown != nullptr && *blockClipDown != ItemID::NONE);
}
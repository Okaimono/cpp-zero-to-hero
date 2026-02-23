#ifndef BLOCK_INTERACTION_H
#define BLOCK_INTERACTION_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <glm/glm.hpp>

#include "Chunk.h"
#include "World.h"
#include "PlayerInventory.h"
#include "GUIManager.h"


class BlockInteraction {
    public:
        PlayerInventory* playerInventory;

        float resetBlockStatus = true;

        inline static float range = 5; 

        BlockInteraction(PlayerInventory* inventory);
    
        void changeHeldBlockType(ItemID blockType);
        Item* turnBlockIntoItem(ItemID blockType, ToolType heldToolType);


        void breakBlock(glm::vec3 Orientation, glm::vec3 playerCoords, World* world);
        bool mineBlock(glm::vec3 Orientation, glm::vec3 playerCoords, World* world, float mineSpeed, float dt);


        ItemID Interact(glm::vec3 Orientation, glm::vec3 playerCoords, World* world, GUIManager* playerGui);

        float getBlockHealth(ItemID blockType);
        float getMultiplier(ItemID blockType, Item* heldItem);

};

#endif
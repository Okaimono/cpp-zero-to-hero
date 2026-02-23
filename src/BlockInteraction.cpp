#include "BlockInteraction.h"

#include <iostream>

BlockInteraction::BlockInteraction(PlayerInventory* inventory)
    : playerInventory(inventory)
{}

float BlockInteraction::getBlockHealth(ItemID blockType) {
    switch (blockType) {
        // Instant break (0 or very low)
        case ItemID::OAK_SAPLING:
        case ItemID::BIRCH_SAPLING:
        case ItemID::DARKOAK_SAPLING:
        case ItemID::JUNGLE_SAPLING:
        case ItemID::SPRUCE_SAPLING:
        case ItemID::WHEAT:
        case ItemID::CARROT:
        case ItemID::POTATO:
        case ItemID::SEEDS:
        case ItemID::ROSE:
        case ItemID::YELLOW_FLOWER:
        case ItemID::TORCH:
        case ItemID::SUGAR_CANE:
            return 0.0f;
        
        // Dirt/Grass blocks (0.75s hand)
        case ItemID::GRASS_BLOCK:
            return 0.9f;

        case ItemID::DIRT_BLOCK:
        case ItemID::SNOW_GRASS_BLOCK:
            return 0.75f;
        
        // Sand/Gravel (0.75s hand)
        case ItemID::SAND_BLOCK:
        case ItemID::GRAVEL_BLOCK:
            return 0.75f;
        
        // Leaves (0.3s hand)
        case ItemID::OAKLEAVES_BLOCK:
        case ItemID::SPRUCELEAVES_BLOCK:
        case ItemID::BIRCHLEAVES_BLOCK:
            return 0.3f;
        
        // Glass (0.3s, instant with any tool)
        
        // Glowstone (0.3s)
        case ItemID::GLOWSTONE_BLOCK:
            return 0.3f;
        
        // Wood logs (2.0s hand)
        case ItemID::OAKLOG_BLOCK:
        case ItemID::SPRUCELOG_BLOCK:
        case ItemID::BIRCHLOG_BLOCK:
            return 2.0f;
        
        // Wood planks (2.0s hand)
        case ItemID::OAKPLANK_BLOCK:
        case ItemID::SPRUCEPLANK_BLOCK:
        case ItemID::BIRCHPLANK_BLOCK:
        case ItemID::CRAFTINGTABLE_BLOCK:
        case ItemID::BARREL_BLOCK:
            return 2.0f;
        
        // Wool (0.8s hand)
        case ItemID::BLACK_WOOL:
        case ItemID::BLUE_WOOL:
        case ItemID::BROWN_WOOL:
        case ItemID::CYAN_WOOL:
        case ItemID::GRAY_WOOL:
        case ItemID::GREEN_WOOL:
        case ItemID::LIGHTBLUE_WOOL:
        case ItemID::LIGHTGRAY_WOOL:
        case ItemID::MAGENTA_WOOL:
        case ItemID::ORANGE_WOOL:
        case ItemID::LIME_WOOL:
        case ItemID::PINK_WOOL:
        case ItemID::PURPLE_WOOL:
        case ItemID::RED_WOOL:
        case ItemID::WHITE_WOOL:
        case ItemID::YELLOW_WOOL:
            return 0.8f;
        
        // TNT (0.0s, instant)
        case ItemID::TNT_BLOCK:
            return 0.0f;
        
        // Melon (1.0s hand)
        case ItemID::MELON_BLOCK:
            return 1.0f;
        
        // Pumpkin (1.0s hand)
        case ItemID::PUMPKIN_BLOCK:
            return 1.0f;
        
        // Cactus (0.4s)
        case ItemID::CACTUS_BLOCK:
            return 0.4f;
        
        // Stone (7.5s hand)
        case ItemID::STONE_BLOCK:
            return 7.5f;
        
        // Cobblestone (7.5s hand)
        case ItemID::COBBLESTONE_BLOCK:
            return 7.5f;
        
        // Stone bricks (7.5s hand)
        case ItemID::STONEBRICK_BLOCK:
            return 7.5f;
        
        // Brick (7.5s hand)
        case ItemID::BRICK_BLOCK:
            return 7.5f;
        
        // Sandstone (4.0s hand)
        case ItemID::SANDSTONE_BLOCK:
            return 4.0f;
        
        // Coal ore (15s hand)
        case ItemID::COAL_ORE:
            return 15.0f;
        
        // Iron ore (15s hand)
        case ItemID::IRON_ORE:
            return 15.0f;
        
        // Gold ore (15s hand)
        case ItemID::GOLD_ORE:
            return 15.0f;
        
        // Diamond ore (15s hand)
        case ItemID::DIAMOND_ORE:
            return 15.0f;
        
        // Emerald ore (15s hand)
        case ItemID::EMERALD_ORE:
            return 15.0f;
        
        // Redstone ore (15s hand)
        case ItemID::REDSTONE_ORE:
            return 15.0f;
        
        // Lapis ore (15s hand)
        case ItemID::LAPIS_ORE:
            return 15.0f;
        
        // Metal blocks (25s hand)
        case ItemID::IRON_BLOCK:
        case ItemID::GOLD_BLOCK:
        case ItemID::DIAMOND_BLOCK:
        case ItemID::EMERALD_BLOCK:
        case ItemID::REDSTONE_BLOCK:
            return 25.0f;
        
        // Obsidian (250s hand, 50s diamond pick)
        case ItemID::OBSIDIAN_BLOCK:
            return 250.0f;
        
        // Bedrock (unbreakable)
        case ItemID::BEDROCK_BLOCK:
            return 99999.0f;
        
        // Furnace (17.5s hand)
        case ItemID::FURNACE_BLOCK:
            return 17.5f;
        
        // Anvil (25s hand)
        case ItemID::ANVIL:
            return 25.0f;
        
        // Enchantment table (25s hand)
        case ItemID::ENCHANTMENT_TABLE_BLOCK:
            return 25.0f;
        
        default:
            return 1.0f;
    }
}

float BlockInteraction::getMultiplier(ItemID blockType, Item* heldItem) {
    if (heldItem == nullptr) return 1.0f;

    ToolType blockBoostType = ToolType::NONE;

    switch (blockType) {
        // --- PICKAXE blocks ---
        case ItemID::STONE_BLOCK:
        case ItemID::COBBLESTONE_BLOCK:
        case ItemID::COAL_ORE:
        case ItemID::IRON_ORE:
        case ItemID::GOLD_ORE:
        case ItemID::EMERALD_ORE:
        case ItemID::DIAMOND_ORE:
        case ItemID::REDSTONE_ORE:
        case ItemID::LAPIS_ORE:
        case ItemID::IRON_BLOCK:
        case ItemID::GOLD_BLOCK:
        case ItemID::DIAMOND_BLOCK:
        case ItemID::EMERALD_BLOCK:
        case ItemID::REDSTONE_BLOCK:
        case ItemID::LAPIS:
        case ItemID::OBSIDIAN_BLOCK:
        case ItemID::BRICK_BLOCK:
        case ItemID::STONEBRICK_BLOCK:
        case ItemID::SANDSTONE_BLOCK:
        case ItemID::FURNACE_BLOCK:
        case ItemID::ANVIL:
        case ItemID::ENCHANTMENT_TABLE_BLOCK:
        case ItemID::CRAFTINGTABLE_BLOCK:
        case ItemID::BARREL_BLOCK:
        case ItemID::BEDROCK_BLOCK:
            blockBoostType = ToolType::PICKAXE;
            break;

        // --- AXE blocks ---
        case ItemID::OAKLOG_BLOCK:
        case ItemID::BIRCHLOG_BLOCK:
        case ItemID::SPRUCELOG_BLOCK:
        case ItemID::OAKPLANK_BLOCK:
        case ItemID::BIRCHPLANK_BLOCK:
        case ItemID::SPRUCEPLANK_BLOCK:
        case ItemID::PUMPKIN_BLOCK:
        case ItemID::MELON_BLOCK:
        case ItemID::JUNGLE_SAPLING:
        case ItemID::OAK_SAPLING:
        case ItemID::BIRCH_SAPLING:
        case ItemID::SPRUCE_SAPLING:
        case ItemID::DARKOAK_SAPLING:
            blockBoostType = ToolType::AXE;
            break;

        // --- SHOVEL blocks ---
        case ItemID::DIRT_BLOCK:
        case ItemID::GRASS_BLOCK:
        case ItemID::SAND_BLOCK:
        case ItemID::GRAVEL_BLOCK:
        case ItemID::SNOW_GRASS_BLOCK:
        case ItemID::SNOW_BALL:
        case ItemID::CLAY_BLOCK:
            blockBoostType = ToolType::SHOVEL;
            break;

        // --- NONE / hand-mineable blocks ---
        case ItemID::TORCH:
        case ItemID::GLASS_BLOCK:
        case ItemID::GLOWSTONE_BLOCK:
        case ItemID::TNT_BLOCK:
        case ItemID::CACTUS_BLOCK:
        case ItemID::ROSE:
        case ItemID::YELLOW_FLOWER:
        case ItemID::WHEAT:
        case ItemID::CARROT:
        case ItemID::POTATO:
        case ItemID::MELON_SLICE:
            blockBoostType = ToolType::NONE;
            break;

        default:
            blockBoostType = ToolType::NONE;
            break;
    }

    // Return boosted speed if correct tool
    if (blockBoostType == heldItem->itemData.toolType)
        return heldItem->itemData.speed;

    return 1.0f;
}


bool BlockInteraction::mineBlock(glm::vec3 Orientation, glm::vec3 playerCoords, World* world, float mineSpeed, float dt)
{
    glm::vec3 rayLocation = playerCoords;
    bool blockMined = false;

    for (float s = 0.1; s <= range; s += 0.1) {
        rayLocation = s * Orientation;
        rayLocation = rayLocation + playerCoords;

        ItemID* block = world->getBlockAtCoords(rayLocation);
        if (block != nullptr && *block != ItemID::NONE) {

            static glm::ivec3 tempBlockPos;
            static float blockHealth;

            static Item** prevHeldItem;

            // Multiplier (changes w/ shovel etc.)
            static float multiplier = 1.0f;
            // Default minespeed (0.9 since its a bit faster than normal)
            const float mineSpeed = 0.85f;

            // THIS IS A PRINT TO GET BLOCK HEALTH OVERTIME
            //if (round(blockHealth) == floor(blockHealth)) {
            //    std::cout << (int)floor(blockHealth) << std::endl;
            //}

            Item** currentHeldItem =  &(playerInventory->backpack[playerInventory->heldItemIndex]);

            if (prevHeldItem != currentHeldItem) {
                resetBlockStatus = true;
            }

            if (resetBlockStatus == true) {
                tempBlockPos = {(int) rayLocation.x, (int) rayLocation.y, (int) rayLocation.z};
                prevHeldItem = currentHeldItem;

                multiplier = getMultiplier(*block, *currentHeldItem);
                blockHealth = getBlockHealth(*block);
                
                resetBlockStatus = false;
            }
            

            glm::ivec3 currentBlockPos = { (int)rayLocation.x, (int)rayLocation.y, (int)rayLocation.z};
            if (currentBlockPos != tempBlockPos) {
                resetBlockStatus = true;
            }


            blockHealth -= mineSpeed * multiplier * dt;

            // If block was mined
            if (blockHealth <= 0) {
                blockMined = true;
                
                ToolType toolType = ToolType::NONE;
                if (*currentHeldItem != nullptr) {
                    toolType = (*currentHeldItem)->itemData.toolType;
                }
                
                Item* newItem = turnBlockIntoItem(*block, toolType);
                
                if (newItem != nullptr) {
                    playerInventory->addSingleItemToInventory(newItem);
                } else {
                }
                
                *block = ItemID::NONE;
                resetBlockStatus = true;

                if (*currentHeldItem != nullptr) {
                    bool isBroken = (*currentHeldItem)->subtractDurability();
                    if (isBroken) {
                        delete *currentHeldItem;
                        *currentHeldItem = nullptr;
                    }
                }
            } 
            else { 
                return false;
            }


            int chunkX = (int)floor(rayLocation.x / 16.0f);
            int chunkZ = (int)floor(rayLocation.z / 16.0f);

            Chunk* ch = world->getChunk({chunkX, chunkZ});
            if (ch) {
                world->updateChunkMeshAt({chunkX, chunkZ});

                float blockX = floor(rayLocation.x) - (chunkX * 16);
                float blockZ = floor(rayLocation.z) - (chunkZ * 16);

                if (blockZ == 0.0f) {
                    world->updateChunkMeshAt({chunkX, chunkZ - 1});
                }
                if (blockX == 15.0f) {
                    world->updateChunkMeshAt({chunkX + 1, chunkZ});
                }
                if (blockZ == 15.0f) {
                    world->updateChunkMeshAt({chunkX, chunkZ + 1});
                }
                if (blockX == 0.0f) {
                    world->updateChunkMeshAt({chunkX - 1, chunkZ});
                }
            }
            break;
        }
    }
    return blockMined;
}

Item* BlockInteraction::turnBlockIntoItem(ItemID blockType, ToolType heldToolType) {
    switch (blockType) {
        // PICKAXE REQUIRED - these blocks drop nothing if mined without pickaxe
        case ItemID::STONE_BLOCK:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(ItemID::COBBLESTONE_BLOCK);
            
        case ItemID::COBBLESTONE_BLOCK:
        case ItemID::STONEBRICK_BLOCK:
        case ItemID::BRICK_BLOCK:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(blockType);
            
        case ItemID::SANDSTONE_BLOCK:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(ItemID::SANDSTONE_BLOCK);
        
        // Ores - REQUIRE pickaxe
        case ItemID::COAL_ORE:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(ItemID::COAL);
            
        case ItemID::IRON_ORE:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(ItemID::IRON_ORE);  // Needs smelting
            
        case ItemID::GOLD_ORE:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(ItemID::GOLD_ORE);  // Needs smelting
            
        case ItemID::DIAMOND_ORE:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(ItemID::DIAMOND);
            
        case ItemID::EMERALD_ORE:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(ItemID::EMERALD);
            
        case ItemID::LAPIS_ORE:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(ItemID::LAPIS, 4 + (rand() % 6));  // 4-9 lapis
            
        case ItemID::REDSTONE_ORE:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(ItemID::REDSTONE, 4 + (rand() % 2));  // 4-5 redstone
        
        // Metal blocks - REQUIRE pickaxe
        case ItemID::IRON_BLOCK:
        case ItemID::GOLD_BLOCK:
        case ItemID::DIAMOND_BLOCK:
        case ItemID::EMERALD_BLOCK:
        case ItemID::REDSTONE_BLOCK:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(blockType);
        
        // Obsidian - REQUIRES diamond pickaxe (for now just require pickaxe)
        case ItemID::OBSIDIAN_BLOCK:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(ItemID::OBSIDIAN_BLOCK);
        
        // Furnace/Anvil - REQUIRE pickaxe
        case ItemID::FURNACE_BLOCK:
        case ItemID::ANVIL:
        case ItemID::ENCHANTMENT_TABLE_BLOCK:
            if (heldToolType != ToolType::PICKAXE) return nullptr;
            return new Item(blockType);
        
        // Grass drops dirt (can mine with anything)
        case ItemID::GRASS_BLOCK:
        case ItemID::SNOW_GRASS_BLOCK:
            return new Item(ItemID::DIRT_BLOCK);
        
        // Dirt/Sand/Gravel - can mine with anything (shovel just faster)
        case ItemID::DIRT_BLOCK:
            return new Item(ItemID::DIRT_BLOCK);
            
        case ItemID::SAND_BLOCK:
            return new Item(ItemID::SAND_BLOCK);

        // Gravel has 10% chance to drop flint
        case ItemID::GRAVEL_BLOCK:
            if (rand() % 100 < 10) {
                return new Item(ItemID::FLINT);
            }
            return new Item(ItemID::GRAVEL_BLOCK);
        case ItemID::CLAY_BLOCK:
            return new Item(ItemID::CLAY_BALL);

        // Wood - can mine with anything (axe just faster)
        case ItemID::OAKLOG_BLOCK:
        case ItemID::SPRUCELOG_BLOCK:
        case ItemID::BIRCHLOG_BLOCK:
        case ItemID::OAKPLANK_BLOCK:
        case ItemID::SPRUCEPLANK_BLOCK:
        case ItemID::BIRCHPLANK_BLOCK:
        case ItemID::CRAFTINGTABLE_BLOCK:
        case ItemID::BARREL_BLOCK:
            return new Item(blockType);
        
        // Leaves drop saplings (5% chance) or nothing
        case ItemID::OAKLEAVES_BLOCK:
            if (rand() % 100 < 5) {
                return new Item(ItemID::OAK_SAPLING);
            }
            return nullptr;
            
        case ItemID::SPRUCELEAVES_BLOCK:
            if (rand() % 100 < 5) {
                return new Item(ItemID::SPRUCE_SAPLING);
            }
            return nullptr;
            
        case ItemID::BIRCHLEAVES_BLOCK:
            if (rand() % 100 < 5) {
                return new Item(ItemID::BIRCH_SAPLING);
            }
            return nullptr;
        
        // Wool - shears drop wool, anything else drops nothing (for now just drop wool)
        case ItemID::BLACK_WOOL:
        case ItemID::BLUE_WOOL:
        case ItemID::BROWN_WOOL:
        case ItemID::CYAN_WOOL:
        case ItemID::GRAY_WOOL:
        case ItemID::GREEN_WOOL:
        case ItemID::LIGHTBLUE_WOOL:
        case ItemID::LIGHTGRAY_WOOL:
        case ItemID::MAGENTA_WOOL:
        case ItemID::ORANGE_WOOL:
        case ItemID::LIME_WOOL:
        case ItemID::PINK_WOOL:
        case ItemID::PURPLE_WOOL:
        case ItemID::RED_WOOL:
        case ItemID::WHITE_WOOL:
        case ItemID::YELLOW_WOOL:
            return new Item(blockType);
        
        // Glowstone drops dust (can mine with anything)
        case ItemID::GLOWSTONE_BLOCK:
            return new Item(ItemID::GLOWSTONE, 2 + (rand() % 3));  // 2-4 dust
        
        // Glass drops nothing (unless silk touch)
        case ItemID::GLASS_BLOCK:
            return nullptr;
        
        // TNT drops itself
        case ItemID::TNT_BLOCK:
            return new Item(ItemID::TNT_BLOCK);
        
        // Melon drops slices (can mine with anything, axe faster)
        case ItemID::MELON_BLOCK:
            return new Item(ItemID::MELON_SLICE, 3 + (rand() % 5));  // 3-7 slices
        
        // Pumpkin
        case ItemID::PUMPKIN_BLOCK:
            return new Item(ItemID::PUMPKIN_BLOCK);
        
        // Cactus
        case ItemID::CACTUS_BLOCK:
            return new Item(ItemID::CACTUS_BLOCK);
        
        // Wheat/crops
        case ItemID::WHEAT:
        case ItemID::CARROT:
        case ItemID::POTATO:
            return new Item(blockType);
        
        // Instant break blocks
        case ItemID::OAK_SAPLING:
        case ItemID::BIRCH_SAPLING:
        case ItemID::DARKOAK_SAPLING:
        case ItemID::JUNGLE_SAPLING:
        case ItemID::SPRUCE_SAPLING:
        case ItemID::SEEDS:
        case ItemID::ROSE:
        case ItemID::YELLOW_FLOWER:
        case ItemID::TORCH:
        case ItemID::SUGAR_CANE:
            return new Item(blockType);
        
        // Default: drop the block itself
        default:
            return new Item(blockType);
    }
}

ItemID BlockInteraction::Interact(glm::vec3 Orientation, glm::vec3 playerCoords, World* world, GUIManager* playerGui)
{
    glm::vec3 rayLocation = playerCoords;

    for (int steps = 1; steps <= (int)(range * 10); steps++) {
        float s = steps * 0.1f;

        rayLocation = s * Orientation;
        rayLocation = rayLocation + playerCoords;

        ItemID* surfaceBlock = world->getBlockAtCoords(rayLocation);
        if (surfaceBlock != nullptr && *surfaceBlock != ItemID::NONE) {

            // If its a crafting table, return that user interacted w/ crafting table
            if (*surfaceBlock == ItemID::CRAFTINGTABLE_BLOCK) 
            {
                return *surfaceBlock;
                break;
            }
            else if (*surfaceBlock == ItemID::BARREL_BLOCK)
            {
                Barrel* newBarrel = world->getBarrelAt(rayLocation);
                playerGui->updateBindedBarrel(newBarrel);
                
                return *surfaceBlock;
                break;
            }
            else if (*surfaceBlock == ItemID::FURNACE_BLOCK)
            {
                Furnace* newFurnace = world->getFurnaceAt(rayLocation);
                playerGui->updateBindedFurnace(newFurnace);

                return *surfaceBlock;
                break;
            }
            // If its a barrel block, return that user interacted w/ barrel
            // After finding surface bock, move back another step.
            rayLocation = Orientation * (s - 0.1f) + playerCoords;

            // The pointer of the block at a specific location
            ItemID* newBlock = world->getBlockAtCoords(rayLocation);

            if (newBlock != nullptr && *newBlock == ItemID::NONE) {
                // Index to held item
                int heldItemIndex = playerInventory->heldItemIndex;
                // Backpack to get held item
                Item** backpack_ptr = playerInventory->backpack;

                // If its nullptr/improper data type.
                if (heldItemIndex < 0 || heldItemIndex >= 36) {continue;}
                if (backpack_ptr[heldItemIndex] == nullptr) continue;

                if ( (*backpack_ptr[heldItemIndex]).isPlaceable() != true) {
                    continue;
                }   
                *newBlock = backpack_ptr[heldItemIndex]->getID();

                if (*newBlock == ItemID::BARREL_BLOCK) {
                    world->addBarrelAt(rayLocation);
                }
                else if (*newBlock == ItemID::FURNACE_BLOCK) {
                    world->addFurnaceAt(rayLocation);
                }

                bool isEmpty = backpack_ptr[heldItemIndex]->subtractOne();
                
                if (isEmpty) {
                    delete backpack_ptr[heldItemIndex];
                    backpack_ptr[heldItemIndex] = nullptr;
                }
            
                int chunkX = (int)floor(rayLocation.x / 16.0f);
                int chunkZ = (int)floor(rayLocation.z / 16.0f);

                world->updateChunkMeshAt({chunkX, chunkZ});
                break;
            }
        }
    }
    return ItemID::NONE;
}
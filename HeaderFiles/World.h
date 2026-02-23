#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "WorldGeneration.h"
#include "EntityManager.h"
#include "JobQueue.h"
#include "ChunkRenderer.h"

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <memory>


// Imports chunk
class Chunk;

using ChunkCoords = std::pair<int, int>;

struct Vec3Hash {
    size_t operator()(const glm::ivec3& v) const {
        return v.x * 73856093 ^ v.y * 19349663 ^ v.z * 83492791;
    }
};

// USE STORAGE OBJECTS IN THE FUTURE!

struct Barrel {
    Item* storage[27];
    
    Barrel() {
        for (int i = 0; i < 27; i++) {
            storage[i] = nullptr;
        }
    } // Constructor with position
};

struct Furnace {
    Item* smeltable;
    Item* fuel;
    Item* result;

    float smeltHP;
    float fuelHP;
    float maxSmeltHP;

    bool smelting = false;

    Furnace() {
        smeltable = nullptr;
        fuel = nullptr;
        result = nullptr;

        maxSmeltHP = 10;
        smeltHP = 10;
        fuelHP = 0;
    }
    void checkToSmelt() {

        if (smeltable != nullptr && fuelHP > 0 &&
            (result == nullptr || result->getQuantity() < result->getMaxQuantity())) 
        { 
            smelting = true;  // ✓ Fixed
        }
        else {
            smelting = false;  // ✓ Fixed
        }
    }

    void setFuelHP(ItemID fuelID) {
        if (fuelID == ItemID::COAL) {
            fuelHP = 80;
        } else if (fuelID == ItemID::OAKLOG_BLOCK) {
            fuelHP = 15;
        } else if (fuelID == ItemID::OAKLOG_BLOCK) {
            fuelHP = 15;
        }
    }

    void smelt(float dt) {
        if (fuelHP <= 0) {
            if (fuel != nullptr) {
                ItemID fuelID = fuel->getID();

                bool isEmpty = fuel->subtractQuantity(1);
                if (isEmpty) {
                    delete fuel;
                    fuel = nullptr;
                }
                setFuelHP(fuelID);
            }
        }
        checkToSmelt();
        if (smelting) {
            smeltHP -= 1 * dt;
            fuelHP -= 1 * dt;


            // If it is smelting and fuel dies

            // If its smelting an item smelts all the way
            if (smeltHP <= 0) {  // ✓ Fixed
                smeltItem();
                smeltHP = maxSmeltHP;
                
                // Consume smeltable ✓ Added
                bool isSmeltableEmpty = smeltable->subtractQuantity(1);

                if (isSmeltableEmpty) {
                    delete smeltable;
                    smeltable = nullptr;
                }
            }
            checkToSmelt();
        }
    }

    // Smelts the item, converts to what item it is, puts it in the result slot
    void smeltItem() {
        ItemID smeltedItem = ItemID::NONE;

        if (smeltable != nullptr) {
            if (smeltable->getID() == ItemID::IRON_ORE) {
                smeltedItem = ItemID::IRON_BAR;
            }
            else if (smeltable->getID() == ItemID::GOLD_ORE) {
                smeltedItem = ItemID::GOLD_BAR;
            }
            else if (smeltable->getID() == ItemID::OAKLOG_BLOCK) {
                smeltedItem = ItemID::COAL;
            }
            else if (smeltable->getID() == ItemID::CLAY_BALL) {
                smeltedItem = ItemID::BRICK;
            }
            // Add more smelting recipes here
        }
        if (result == nullptr) {
            result = new Item(smeltedItem);
        }
        else if (result->getID() == smeltedItem &&  // ✓ Fixed
            result->getQuantity() < result->getMaxQuantity())
        {
            result->addQuantity(1);
        }

        if (result->getQuantity() == result->getMaxQuantity()) {
            smelting = false;
        }
    }
};

class World
{
    public:
        ChunkCoords userChunkPosition;
        EntityManager entityManager;
        ChunkRenderer chunkRenderer;
        glm::vec3* Position;
        std::atomic<int> worldGenerationCount{0};

        float timeOfDay = 0.0f;  // 0.0 = midnight, 0.5 = noon, 1.0 = midnight
        float dayLength = 120.0f;  // seconds (2 minutes for testing, change to 1200 later)

        World(int rendDist);

        void Delete();
        void updateChunkMeshAt(ChunkCoords coords);
        void createChunk(ChunkCoords chunkCoords);
        void gameTick(float dt);
        void gameSmeltTick(float dt);
        void gameMobTick(float dt);
        void createChunkMeshes();
        void initializeWorld();
        void initializeEntityManager(Camera* cam);
        void resetRenderArea();
        void cleanRenderAreva();
        void renderWorld(const glm::mat4& camMatrix);
        void renderMobs();
        void explodeAtCoords(glm::vec3 center, float radius);
        void updateMeshWithBlock(glm::vec3 blockCoords);
        void addFurnaceAt(glm::vec3 coords);
        void addBarrelAt(glm::vec3 coords);
        void genChunkThread(JobQueue &jobQueue);
        void constructChunk(ChunkCoords coords);
        void createChunkMesh(ChunkCoords coords);
        void completeTask(const ChunkJob& chunkJob);
        void pushMeshJobs();
        void debug() {std::cout << "TEST" << "\n";}

        Barrel* getBarrelAt(glm::vec3 coords);
        Furnace* getFurnaceAt(glm::vec3 coords);
        Chunk* getChunk(ChunkCoords chunkCoords);
        ItemID* getBlockAtCoords(glm::vec3 coords);

        const std::unordered_map<ChunkCoords, Chunk, PairHash>& getWorldGrid();
    private:
        JobQueue chunkJobQueue;

        std::mutex worldGridMutex;
        std::unordered_map<ChunkCoords, Chunk, PairHash> worldGrid;

        std::mutex checkIfExistsMutex;
        std::unordered_set<ChunkCoords, PairHash> checkIfExists;
        
        std::unordered_map<glm::ivec3, Barrel, Vec3Hash> worldBarrels;
        std::unordered_map<glm::ivec3, Furnace, Vec3Hash> worldFurnaces;


        std::mutex completedChunkMutex;
        std::queue<ChunkCoords> completedChunks;
 
        std::vector<ChunkCoords> renderArea;

        std::vector<std::thread> workers;

        WorldGeneration worldGenerator;

        const int renderDist;
};

#endif
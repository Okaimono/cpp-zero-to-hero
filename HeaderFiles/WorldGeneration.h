#ifndef WORLD_GENERATION_H
#define WORLD_GENERATION_H

#include "Chunk.h"
#include "EntityManager.h"
#include "NoiseGenerator.h"

#include <utility>

class World;

using ChunkCoords = std::pair<int, int>;

enum class BiomeType { GRASSY_PLAINS, MOUNTAINS, SPRUCE, OAK };
enum class TreeType { OAK, SPRUCE };

class WorldGeneration {
public:
    WorldGeneration(int s, int cs, int ch, World* world, EntityManager& em);

    void generateChunk(ItemID p_blocks[16][256][16], ChunkCoords chunkCoords);

private:
    EntityManager& entityManager;
    World* world;

    ItemID oreLUT[256][256];  // maps [y][oreIndex] → ItemID
    float oreYJitter[256];   // per-Y noise warp
    
    const int seed;
    const int chunkSize;
    const int chunkHeight;
    
    NoiseGenerator noise;  // FastNoiseLite wrapper

    void generateTrees(ItemID(*p_blocks)[256][16], TreeType treeType, int x, int y, int z);

    float depthFallOff(int y);
    float lerp(float a, float b, float t);
    void buildOreLUT();
};

#endif
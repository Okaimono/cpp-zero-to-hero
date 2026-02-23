#include "WorldGeneration.h"
#include "Chunk.h"
#include "Profiler.h"

#include <cstdlib>
#include <algorithm>

WorldGeneration::WorldGeneration(int s, int cs, int ch, World* w, EntityManager& em)
    : seed(s),
      chunkSize(cs),
      chunkHeight(ch),
      world(w),
      entityManager(em),
      noise(s)
{}

void WorldGeneration::generateChunk(ItemID p_blocks[16][256][16], ChunkCoords chunkCoords) 
{
    float biomeNoiseMap[16][16];
    float forestNoiseMap[16][16];
    float worldHeightMap[16][16];

    float oreNoiseMap[16][16][256];

    const float chunkPosX = chunkCoords.first * chunkSize;
    const float chunkPosZ = chunkCoords.second * chunkSize;


    for (int x = 0; x < chunkSize; x++) {
        for (int z = 0; z < chunkSize; z++) {
            const float coordX = x + chunkPosX;
            const float coordZ = z + chunkPosZ;

            // Get noise values using FastNoiseLite
            float biomeNoise = noise.getBiomeNoise(coordX, coordZ);
            float forestNoise = noise.getForestNoise(coordX, coordZ);

            biomeNoiseMap[x][z] = biomeNoise;
            forestNoiseMap[x][z] = forestNoise;

            // Height calculation - exact same logic as before
            if (biomeNoise < -0.5f) {
                worldHeightMap[x][z] = biomeNoise * 20.0f;
            }
            else if (biomeNoise < 0.0f) {
                float plainsHeight = biomeNoise * 20.0f;  // Reuse biomeNoise
                float forestHeight = forestNoise * 50.0f + 5.0f;
                float t = (biomeNoise + 0.5f) / 0.5f;
                worldHeightMap[x][z] = lerp(plainsHeight, forestHeight, t);
            }
            else if (biomeNoise < 0.5f) {
                float forestHeight = forestNoise * 50.0f + 5.0f;
                float mountainHeight = biomeNoise * 120.0f + 80.0f;  // Reuse biomeNoise
                float t = biomeNoise / 0.5f;
                worldHeightMap[x][z] = lerp(forestHeight, mountainHeight, t);
            }
            else {
                worldHeightMap[x][z] = biomeNoise * 120.0f + 80.0f;
            }

            for (int y = 1; y < worldHeightMap[x][z]; y++) {
                oreNoiseMap[x][z][y] = noise.getOreNoise(coordX, coordZ, y); 
            }
        }
    }

    // Stage 2: Generate terrain
    for (int x = 0; x < chunkSize; x++) {    
        for (int z = 0; z < chunkSize; z++) {

            TreeType treeType;
            bool genForests = false;
            bool genMob = false;
            
            const float coordX = x + chunkPosX;
            const float coordZ = z + chunkPosZ;

            genMob = rand() % 250 == 0;

            float biomeNoise = biomeNoiseMap[x][z];

            // Tree type selection - same logic
            if (biomeNoise > -0.5f && biomeNoise < 0.0f) {
                treeType = TreeType::OAK;
                genForests = true;
            }
            else if (biomeNoise < 0.5f) {
                treeType = TreeType::SPRUCE;
                genForests = true;
            }

            int finalHeight = static_cast<int>(worldHeightMap[x][z]) + 64;
            finalHeight = std::clamp(finalHeight, 0, chunkHeight - 1);

            p_blocks[x][0][z] = ItemID::BEDROCK_BLOCK;

            for (int y = 1; y < finalHeight - 6; y++) {
                p_blocks[x][y][z] = ItemID::STONE_BLOCK;
            }
            for (int y = finalHeight - 6; y < finalHeight; y++) {
                p_blocks[x][y][z] = ItemID::DIRT_BLOCK;
            }

            if (finalHeight >= 100) p_blocks[x][finalHeight][z] = ItemID::SNOW_GRASS_BLOCK;
            else p_blocks[x][finalHeight][z] = ItemID::GRASS_BLOCK;

            for (int y = finalHeight + 1; y < 63; y++) {
                p_blocks[x][y][z] = ItemID::WATER_BLOCK;
            }

            // Fill column
            for (int y = 1; y < finalHeight - 6; y++) {
                ItemID oreId = ItemID::STONE_BLOCK;
                float oreValue =  oreNoiseMap[x][z][y];

                if (oreValue <= 0.45f) {
                    // stays stone
                }
                else if (y <= 16 && oreValue > 0.67f) {
                    oreId = ItemID::DIAMOND_ORE;
                }
                else if (y <= 20) {
                    if (oreValue > 0.64f) oreId = ItemID::LAPIS_ORE;
                    else if (oreValue > 0.62f) oreId = ItemID::REDSTONE_ORE;
                }
                else if (y <= 32) {
                    if (oreValue > 0.61f) oreId = ItemID::GOLD_ORE;
                }
                else if (y <= 60) {
                    if (oreValue > 0.55f) oreId = ItemID::IRON_ORE;
                }
                else if (y >= 35) {
                    if (oreValue > 0.45f) oreId = ItemID::COAL_ORE;
                }
                if (oreId != ItemID::STONE_BLOCK)
                    p_blocks[x][y][z] = oreId;
            }

            // Tree generation
            if (genForests) {
                if (x > 1 && x < 14 && z > 1 && z < 14) {
                    if (p_blocks[x][finalHeight][z] == ItemID::GRASS_BLOCK && 
                        p_blocks[x][finalHeight + 1][z] != ItemID::WATER_BLOCK) {
                        
                        float treeNoiseValue = noise.getTreeNoise(coordX, coordZ);
                        int randGen = rand() % 11;
                        
                        if (treeNoiseValue > 0 && randGen == 0) {
                            generateTrees(p_blocks, treeType, x, finalHeight, z);
                        }
                    }
                }
            }

            if (false && genMob) {
                entityManager.generateMob(glm::vec3(coordX, finalHeight + 5.0f, coordZ));
            }
        }
    }
}

void WorldGeneration::generateTrees(ItemID p_blocks[16][256][16], TreeType tree, int x, int y, int z) {
    if (tree == TreeType::OAK) {
        // Trunk
        for (int dy = 1; dy < 5; dy++) {
            p_blocks[x][y + dy][z] = ItemID::OAKLOG_BLOCK;
        }
        // Lower leaves
        for (int dy = 0; dy < 2; dy++) {
            for (int dx = -2; dx <= 2; dx++) {
                for (int dz = -2; dz <= 2; dz++) {
                    int xPos = x + dx;
                    int yPos = y + 4 + dy;
                    int zPos = z + dz;
                    if (p_blocks[xPos][yPos][zPos] == ItemID::NONE) {
                        p_blocks[xPos][yPos][zPos] = ItemID::OAKLEAVES_BLOCK;
                    }
                }
            }
        }
        // Upper leaves
        for (int dy = 0; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                for (int dz = -1; dz <= 1; dz++) {
                    int xPos = x + dx;
                    int yPos = y + 6 + dy;
                    int zPos = z + dz;
                    if (p_blocks[xPos][yPos][zPos] == ItemID::NONE) {
                        p_blocks[xPos][yPos][zPos] = ItemID::OAKLEAVES_BLOCK;
                    }
                }
            }
        }
    }
    // Add SPRUCE case if needed
}

float WorldGeneration::lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float WorldGeneration::depthFallOff(int y) {
    float yf = float(y) / 256.0f;
    return yf * yf * (3 - 2*yf);
}
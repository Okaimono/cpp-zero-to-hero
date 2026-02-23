#include "Chunk.h"
#include "World.h"

#include <glm/glm.hpp>
#include <iostream>
#include <cmath>

/*
WHOLE PROCESS:
1. Create MeshData in chunk
2. When called to upload to mesh, simply upload to VAO/VBO, and bam.
*/

Chunk::Chunk(std::pair<int, int> cc, int cs, int ch, WorldGeneration& terrainGenerator, World* w)
    : chunkCoords(cc), 
    chunkSize(cs),
    chunkHeight(ch), 
    world(w),
    chunkDataMutex(std::make_unique<std::mutex>())
{
    terrainGenerator.generateChunk(blocks, chunkCoords);
    vertices.reserve(20000);
    indices.reserve(20000);
}

void Chunk::deleteMeshData() {
    {
        std::lock_guard<std::mutex> lock(*chunkDataMutex);
        vertices.clear();
        indices.clear();
        generatingChunk = false;
        completeMesh = false;
    }
}

void Chunk::constructMeshData(int generation) {
    {
        std::lock_guard<std::mutex> lock(*chunkDataMutex);
        generatingChunk = true;
        completeMesh = false;
    }
    vertices.clear();
    indices.clear();
    
    Chunk* northChunk = world->getChunk({chunkCoords.first, chunkCoords.second - 1});  // -Z
    Chunk* eastChunk = world->getChunk({chunkCoords.first + 1, chunkCoords.second});   // +X
    Chunk* southChunk = world->getChunk({chunkCoords.first, chunkCoords.second + 1});  // +Z
    Chunk* westChunk = world->getChunk({chunkCoords.first - 1, chunkCoords.second});   // -X

    // Clears any left over data in the vertices.
    for (int x = 0; x < chunkSize; x++) {
        if (world->worldGenerationCount != generation)
        {
            vertices.clear();
            indices.clear();
            //std::cout << "ERROR: " << "CHUNK GEN: " << generation << ", WORLD GEN: " << world->worldGenerationCount << "\n";
            return;
        }
        for (int y = 0; y < chunkHeight; y++) {
            for (int z = 0; z < chunkSize; z++) {
                bool shouldRestart = false;
                {
                    std::lock_guard<std::mutex> lock(*chunkDataMutex);
                    if (restart) {
                        shouldRestart = true;
                        restart = false;
                    }
                }

                ItemID blockType = blocks[x][y][z];

                // If block is not air, render it.
                if (blockType != ItemID::NONE) {
                    // Add top face if block above is air.  
                    if (y == chunkHeight - 1 || blocks[x][y + 1][z] == ItemID::NONE)
                        addTopFace(x, y, z, blockType);

                    // Add bottom face if block below is air.
                    if (y == 0 || blocks[x][y - 1][z] == ItemID::NONE)
                        addBottomFace(x, y, z, blockType);  

                    // Adds north face if north block is air.
                    if ((z == 0 && (!northChunk || northChunk->blocks[x][y][chunkSize - 1] == ItemID::NONE)) || (z != 0 && blocks[x][y][z - 1] == ItemID::NONE))
                        addNorthFace(x, y, z, blockType);
                    
                    // Add south face if block above is air.
                    if ((z == chunkSize - 1 && (!southChunk || southChunk->blocks[x][y][0] == ItemID::NONE)) || (z != chunkSize - 1 && blocks[x][y][z + 1] == ItemID::NONE))
                        addSouthFace(x, y, z, blockType);

                    // Add west face if block above is air.
                    if ((x == 0 && (!westChunk || westChunk->blocks[chunkSize - 1][y][z] == ItemID::NONE)) || (x != 0 && blocks[x - 1][y][z] == ItemID::NONE) )
                        addWestFace(x, y, z, blockType);
                    
                    // Add east face if block above is air.
                    if ((x == chunkSize - 1 && (!eastChunk || eastChunk->blocks[0][y][z] == ItemID::NONE)) || (x != chunkSize - 1 && blocks[x + 1][y][z] == ItemID::NONE))
                        addEastFace(x, y, z, blockType);
                }
            }
        }
    }
    std::lock_guard<std::mutex> lock(*chunkDataMutex);
    generatingChunk = false;
    completeMesh = true;
}

void Chunk::generateChunkMesh()
{

}

void Chunk::render()
{
    
}

void Chunk::addTopFace(int x, int y, int z, ItemID blockType)
{
    // Vertex coordinates to create a side
    glm::vec3 v0 = {x - 0.5f, y + 0.5f, z - 0.5f}; // Topleft (close) vertex
    glm::vec3 v1 = {x + 0.5f, y + 0.5f, z - 0.5f}; // TopRight (close) vertex
    glm::vec3 v2 = {x + 0.5f, y + 0.5f, z + 0.5f}; // Top
    glm::vec3 v3 = {x - 0.5f, y + 0.5f, z + 0.5f};

    // UV coordinates to be used to implement texture on side
    glm::vec2* uvs = getUVCoords(blockType, FaceType::TOP);
    addQuad(v0, v1, v2, v3, uvs[0], uvs[1], uvs[2], uvs[3]);
}

void Chunk::addBottomFace(int x, int y, int z, ItemID blockType)
{
    glm::vec3 v0 = {x - 0.5f, y - 0.5f, z - 0.5f}; 
    glm::vec3 v1 = {x + 0.5f, y - 0.5f, z - 0.5f};
    glm::vec3 v2 = {x + 0.5f, y - 0.5f, z + 0.5f};
    glm::vec3 v3 = {x - 0.5f, y - 0.5f, z + 0.5f};

    glm::vec2* uvs = getUVCoords(blockType, FaceType::BOTTOM);
    addQuad(v0, v1, v2, v3, uvs[0], uvs[1], uvs[2], uvs[3]);
}

void Chunk::addNorthFace(int x, int y, int z, ItemID blockType)
{
    // North face is at z - 0.5f (looking toward -Z)
    glm::vec3 v0 = {x - 0.5f, y - 0.5f, z - 0.5f};  // Changed from z + 0.5f
    glm::vec3 v1 = {x + 0.5f, y - 0.5f, z - 0.5f};  // Changed from z + 0.5f
    glm::vec3 v2 = {x + 0.5f, y + 0.5f, z - 0.5f};  // Changed from z + 0.5f
    glm::vec3 v3 = {x - 0.5f, y + 0.5f, z - 0.5f};  // Changed from z + 0.5f


    glm::vec2* uvs = getUVCoords(blockType, FaceType::NORTH);
    addQuad(v0, v1, v2, v3, uvs[0], uvs[1], uvs[2], uvs[3]);
}

void Chunk::addEastFace(int x, int y, int z, ItemID blockType)
{
    glm::vec3 v0 = {x + 0.5f, y - 0.5f, z + 0.5f}; 
    glm::vec3 v1 = {x + 0.5f, y - 0.5f, z - 0.5f};
    glm::vec3 v2 = {x + 0.5f, y + 0.5f, z - 0.5f};
    glm::vec3 v3 = {x + 0.5f, y + 0.5f, z + 0.5f};


    glm::vec2* uvs = getUVCoords(blockType, FaceType::EAST);
    addQuad(v0, v1, v2, v3, uvs[0], uvs[1], uvs[2], uvs[3]);
}

void Chunk::addSouthFace(int x, int y, int z, ItemID blockType)
{
    // South face is at z + 0.5f (looking toward +Z)
    glm::vec3 v0 = {x + 0.5f, y - 0.5f, z + 0.5f};  // Changed from z - 0.5f
    glm::vec3 v1 = {x - 0.5f, y - 0.5f, z + 0.5f};  // Changed from z - 0.5f
    glm::vec3 v2 = {x - 0.5f, y + 0.5f, z + 0.5f};  // Changed from z - 0.5f
    glm::vec3 v3 = {x + 0.5f, y + 0.5f, z + 0.5f};  // Changed from z - 0.5f

    glm::vec2* uvs = getUVCoords(blockType, FaceType::SOUTH);
    addQuad(v0, v1, v2, v3, uvs[0], uvs[1], uvs[2], uvs[3]);
}

void Chunk::addWestFace(int x, int y, int z, ItemID blockType)
{
    glm::vec3 v0 = {x - 0.5f, y - 0.5f, z - 0.5f}; 
    glm::vec3 v1 = {x - 0.5f, y - 0.5f, z + 0.5f};
    glm::vec3 v2 = {x - 0.5f, y + 0.5f, z + 0.5f};
    glm::vec3 v3 = {x - 0.5f, y + 0.5f, z - 0.5f};

    glm::vec2* uvs = getUVCoords(blockType, FaceType::WEST);
    addQuad(v0, v1, v2, v3, uvs[0], uvs[1], uvs[2], uvs[3]);
}

void Chunk::setAtlasUVCoords(glm::vec2* uvs, int xPos, int yPos)
{
    xPos--;
    yPos--;
    static const int atlasWidth = 32;
    static const int atlasHeight = 32;
    static const float atlasWidthDist = 1.0 / atlasWidth;
    static const float atlasHeightDist = 1.0 / atlasHeight;

    uvs[0] = {0.0f, atlasHeightDist};
    uvs[1] = {atlasWidthDist, atlasHeightDist};
    uvs[2] = {atlasWidthDist, 0.0f};
    uvs[3] = {0.0f, 0.0f};

    glm::vec2 bCoords = {xPos * atlasWidthDist, yPos * atlasHeightDist};

    uvs[0] += bCoords;
    uvs[1] += bCoords;
    uvs[2] += bCoords;
    uvs[3] += bCoords;
}

void Chunk::addQuad(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,
                    glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3) {

    v0.x += 0.5;
    v1.x += 0.5;
    v2.x += 0.5;
    v3.x += 0.5;

    v0.y += 0.5;
    v1.y += 0.5;
    v2.y += 0.5;
    v3.y += 0.5;

    v0.z += 0.5;
    v1.z += 0.5;
    v2.z += 0.5;
    v3.z += 0.5;
    
    uint32_t base = vertices.size();
    vertices.insert(vertices.end(), {
        {v0, uv0},
        {v1, uv1},
        {v2, uv2},
        {v3, uv3}
    });
    
    indices.insert(indices.end(), {
        base, base+1, base+2,
        base, base+2, base+3
    });
}

// Try to refractor this later
glm::vec2* Chunk::getUVCoords(ItemID blockType, FaceType face)
{
    int blockAtlasWidth = 32;
    int blockAtlasLength = 32;

    float xPos = 1.0f / blockAtlasWidth;
    float yPos = 1.0f / blockAtlasLength;

    uvs[0] = {0.0f, yPos};
    uvs[1] = {xPos, yPos};
    uvs[2] = {xPos, 0.0f};
    uvs[3] = {0.0f, 0.0f};


    if (blockType == ItemID::GRASS_BLOCK) {
        // Sets the grass block's top
        if (face == FaceType::TOP)
            setAtlasUVCoords(uvs, 1, 1);

        // Sets the grass block's bottom
        else if (face == FaceType::BOTTOM)
            setAtlasUVCoords(uvs, 1, 3);

        // Sets the grass block's side
        else  // NORTH, SOUTH, EAST, WEST - all grass side
            setAtlasUVCoords(uvs, 1, 2);
    }
    // Cobblestone
    else if (blockType == ItemID::COBBLESTONE_BLOCK) {
        setAtlasUVCoords(uvs, 2, 1);
    }
    else if (blockType == ItemID::COAL_ORE)
        setAtlasUVCoords(uvs, 3, 1);

    else if (blockType == ItemID::IRON_ORE)
        setAtlasUVCoords(uvs, 4, 1);

    else if (blockType == ItemID::GOLD_ORE)
        setAtlasUVCoords(uvs, 5, 1);

    else if (blockType == ItemID::EMERALD_ORE)
        setAtlasUVCoords(uvs, 6, 1);

    else if (blockType == ItemID::EMERALD_BLOCK)
        setAtlasUVCoords(uvs, 7, 1);

    else if (blockType == ItemID::DIAMOND_BLOCK)
        setAtlasUVCoords(uvs, 8, 1);

    else if (blockType == ItemID::GOLD_BLOCK)
        setAtlasUVCoords(uvs, 9, 1);

    else if (blockType == ItemID::REDSTONE_BLOCK)
        setAtlasUVCoords(uvs, 10, 1);

    else if (blockType == ItemID::IRON_BLOCK)
        setAtlasUVCoords(uvs, 11, 1);

    else if (blockType == ItemID::STONE_BLOCK) {
        setAtlasUVCoords(uvs, 2, 2);
    }

    else if (blockType == ItemID::DIAMOND_ORE) {
        setAtlasUVCoords(uvs, 3, 2);
    }

    else if (blockType == ItemID::REDSTONE_ORE) {
        setAtlasUVCoords(uvs, 4, 2);
    }
    else if (blockType == ItemID::LAPIS_ORE) {
        setAtlasUVCoords(uvs, 5, 2);
    }
    else if (blockType == ItemID::SNOW_GRASS_BLOCK) {
        if (face == FaceType::TOP)
            setAtlasUVCoords(uvs, 7, 2);
        else if (face == FaceType::BOTTOM)
            setAtlasUVCoords(uvs, 1, 3);
        else  // NORTH, SOUTH, EAST, WEST - all grass side
            setAtlasUVCoords(uvs, 6, 2);
    }
    else if (blockType == ItemID::DIRT_BLOCK) {
        setAtlasUVCoords(uvs, 1, 3);
    }
    else if (blockType == ItemID::OAKPLANK_BLOCK) {
        setAtlasUVCoords(uvs, 2, 3);
    }
    else if (blockType == ItemID::SPRUCEPLANK_BLOCK) {
        setAtlasUVCoords(uvs, 3, 3);
    }
    else if (blockType == ItemID::BIRCHPLANK_BLOCK) {
        setAtlasUVCoords(uvs, 4, 3);
    }
    else if (blockType == ItemID::WATER_BLOCK) {
        setAtlasUVCoords(uvs, 5, 3);
    }
    else if (blockType == ItemID::BEDROCK_BLOCK) {
        setAtlasUVCoords(uvs, 6, 3);
    }
    else if (blockType == ItemID::OAKLOG_BLOCK) {
        // If it is the bottom/top of oak log
        if (face == FaceType::TOP || face == FaceType::BOTTOM)
            setAtlasUVCoords(uvs, 2, 4);
        // If it is the side of oak log
        else
            setAtlasUVCoords(uvs, 1, 4);
    }
    else if (blockType == ItemID::OAKLEAVES_BLOCK) {
        setAtlasUVCoords(uvs,3, 4);
    }
    else if (blockType == ItemID::SAND_BLOCK) {
        setAtlasUVCoords(uvs, 4, 4);
    }
    else if (blockType == ItemID::LAVA_BLOCK) {
        setAtlasUVCoords(uvs, 5, 4);
    }
    else if (blockType == ItemID::OBSIDIAN_BLOCK) {
        setAtlasUVCoords(uvs, 6, 4);
    }





    else if (blockType == ItemID::GLOWSTONE_BLOCK) {
        setAtlasUVCoords(uvs, 7, 4);
    }
    else if (blockType == ItemID::SPRUCELOG_BLOCK) {
        if (face == FaceType::TOP || face == FaceType::BOTTOM)
            setAtlasUVCoords(uvs, 2, 5);
        // If it is the side of oak log
        else
            setAtlasUVCoords(uvs, 1, 5);
    }
    else if (blockType == ItemID::SPRUCELEAVES_BLOCK) {
        setAtlasUVCoords(uvs, 3, 5);
    }
    else if (blockType == ItemID::GLASS_BLOCK) {
        setAtlasUVCoords(uvs, 4, 5);
    }
    else if (blockType == ItemID::BRICK_BLOCK) {
        setAtlasUVCoords(uvs, 5, 5);
    }
    else if (blockType == ItemID::STONEBRICK_BLOCK) {
        setAtlasUVCoords(uvs, 6, 5);
    }
    else if (blockType == ItemID::SANDSTONE_BLOCK) {
        if (face == FaceType::TOP)
            setAtlasUVCoords(uvs, 9, 5);
        // Sets the grass block's bottom
        else if (face == FaceType::BOTTOM)
            setAtlasUVCoords(uvs, 8, 5);
        // Sets the grass block's side
        else  // NORTH, SOUTH, EAST, WEST - all grass side
            setAtlasUVCoords(uvs, 7, 5);
    }
    else if (blockType == ItemID::BIRCHLOG_BLOCK) {
        if (face == FaceType::TOP || face == FaceType::BOTTOM)
            setAtlasUVCoords(uvs, 2, 6);
        // If it is the side of oak log
        else
            setAtlasUVCoords(uvs, 1, 6);
    }
    else if (blockType == ItemID::BIRCHLEAVES_BLOCK) {
        setAtlasUVCoords(uvs, 3, 6);
    } 
    else if (blockType == ItemID::CACTUS_BLOCK) {
        if (face == FaceType::TOP || face == FaceType::BOTTOM)
            setAtlasUVCoords(uvs, 5, 6);
        // If it is the side of oak log
        else
            setAtlasUVCoords(uvs, 4, 6);
    }
    else if (blockType == ItemID::GRAVEL_BLOCK) {
        setAtlasUVCoords(uvs, 6, 6);
    }
    else if (blockType == ItemID::CRAFTINGTABLE_BLOCK) {
        if (face == FaceType::TOP)
            setAtlasUVCoords(uvs, 3, 7);
        // Sets the grass block's bottom
        else if (face == FaceType::BOTTOM)
            setAtlasUVCoords(uvs, 2, 3);
        else if (face == FaceType::NORTH || face == FaceType::SOUTH)
            setAtlasUVCoords(uvs, 1, 7);
        // Sets the grass block's side
        else  // NORTH, SOUTH, EAST, WEST - all grass side
            setAtlasUVCoords(uvs, 2, 7);
    }
    else if (blockType == ItemID::FURNACE_BLOCK) {
        if (face == FaceType::TOP || face == FaceType::BOTTOM)
            setAtlasUVCoords(uvs, 4, 8);
        // Sets the grass block's bottom
        else if (face == FaceType::NORTH || face == FaceType::SOUTH)
            setAtlasUVCoords(uvs, 1, 8);
        // Sets the grass block's side
        else  // NORTH, SOUTH, EAST, WEST - all grass side
            setAtlasUVCoords(uvs, 3, 8);
    }
    else if (blockType == ItemID::BARREL_BLOCK) {
        if (face == FaceType::TOP || face == FaceType::BOTTOM)
            setAtlasUVCoords(uvs, 1, 9);
            //setAtlasUVCoords(uvs, 1, 9);
        else
            setAtlasUVCoords(uvs, 3, 9);
    }



    // Wool
    else if (blockType == ItemID::BLACK_WOOL) {
        setAtlasUVCoords(uvs, 1, 10);
    }
    else if (blockType == ItemID::BLUE_WOOL) {
        setAtlasUVCoords(uvs, 2, 10);
    }
    else if (blockType == ItemID::BROWN_WOOL) {
        setAtlasUVCoords(uvs, 3, 10);
    }
    else if (blockType == ItemID::CYAN_WOOL) {
        setAtlasUVCoords(uvs, 4, 10);
    }
    else if (blockType == ItemID::GRAY_WOOL) {
        setAtlasUVCoords(uvs, 5, 10);
    }
    else if (blockType == ItemID::GREEN_WOOL) {
        setAtlasUVCoords(uvs, 6, 10);
    }
    else if (blockType == ItemID::LIGHTBLUE_WOOL) {
        setAtlasUVCoords(uvs, 1, 11);
    }
    else if (blockType == ItemID::LIGHTGRAY_WOOL) {
        setAtlasUVCoords(uvs, 2, 12);
    }
    else if (blockType == ItemID::MAGENTA_WOOL) {
        setAtlasUVCoords(uvs, 4, 11);
    }
    else if (blockType == ItemID::ORANGE_WOOL) {
        setAtlasUVCoords(uvs, 5, 11);
    }
    else if (blockType == ItemID::LIME_WOOL) {
        setAtlasUVCoords(uvs, 3, 11);
    }
    else if (blockType == ItemID::PINK_WOOL) {
        setAtlasUVCoords(uvs, 6, 11);
    }
    else if (blockType == ItemID::PURPLE_WOOL) {
        setAtlasUVCoords(uvs, 1, 12);
    }
    else if (blockType == ItemID::RED_WOOL) {
        setAtlasUVCoords(uvs, 2, 12);
    }
    else if (blockType == ItemID::WHITE_WOOL) {
        setAtlasUVCoords(uvs, 3, 12);
    }
    else if (blockType == ItemID::YELLOW_WOOL) {
        setAtlasUVCoords(uvs, 4, 12);
    }
    else if (blockType == ItemID::TNT_BLOCK) {
        setAtlasUVCoords(uvs, 1, 13);
    }
    else if (blockType == ItemID::CLAY_BLOCK) {
        setAtlasUVCoords(uvs, 7, 3);
    }

    return uvs;
}
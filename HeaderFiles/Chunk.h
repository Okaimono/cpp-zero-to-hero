#ifndef CHUNK_H
#define CHUNK_H

#include"Item.h"
#include"ChunkRenderer.h"

#include <vector>
#include <utility>
#include <memory>
#include <atomic>
#include <mutex>

class World;
class WorldGeneration;

using ChunkCoords = std::pair<int, int>;

enum class FaceType {TOP, BOTTOM, NORTH, EAST, SOUTH, WEST};

class Chunk 
{
    public:
        ItemID blocks[16][256][16] = {};
        ChunkCoords chunkCoords;

        std::unique_ptr<std::mutex> chunkDataMutex;
        bool restart = false;
        bool generatingChunk = false;
        bool completeMesh = false;
        
        // Work in process chunk constructor
        Chunk(ChunkCoords ch, int chunkSize, int chunkHeight, WorldGeneration& terrainGenerator, World* w);
        Chunk() = default;

        Chunk(Chunk&&) = default;
        Chunk& operator=(Chunk&&) = default;
        
        // TODO: Refractor
        const std::vector<Vertex>& getVertices() const {return vertices;}
        const std::vector<GLuint>& getIndices() const {return indices;}

        void generateTopLayer();
        void constructMeshData(int generation);

        void generateChunkMesh();
        void deleteMeshData();
        void render();

        // TODO: Remove
        int hash(int x, int z, int seed);
        float fade(float t);
        float lerp(float a, float b, float t);
        float gradient(int x, int z, float dx, float dz, int seed);
        float noise(float x, float z, int seed);
        float getHeight(float x, float z, int seed);

    private:
        World* world;
        glm::vec2 uvs[4];

        int chunkSize, chunkHeight;


        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        void addBlocks();

        void addQuad( glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, 
            glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3);  // 4 UVs);
            
        void addTopFace(int x, int y, int z, ItemID blockType);
        void addBottomFace(int x, int y, int z, ItemID blockType);
        void addNorthFace(int x, int y, int z, ItemID blockType);
        void addEastFace(int x, int y, int z, ItemID blockType);
        void addSouthFace(int x, int y, int z, ItemID blockType);
        void addWestFace(int x, int y, int z, ItemID blockType);

        glm::vec2* getUVCoords(ItemID blockType, FaceType face);
        void setAtlasUVCoords(glm::vec2* uvs, int xPos, int yPos);
};

#endif
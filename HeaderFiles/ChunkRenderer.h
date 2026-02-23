#ifndef CHUNK_RENDERER_H
#define CHUNK_RENDERER_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"

#include <array>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utility>



using ChunkCoords = std::pair<int, int>;

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
};

static constexpr size_t SIZEOF_VERTEX = sizeof(Vertex);
static constexpr size_t SIZEOF_INDEX = sizeof(GLuint);

struct PairHash {
    size_t operator()(const std::pair<int, int>& p) const {
        return p.first * 100000 + p.second;
    }
};

class ChunkRenderer {
public:
    VAO vao;
    VBO vbo;
    EBO ebo;

    static constexpr size_t GiB = 1024ull * 1024ull * 1024ull;
    static constexpr size_t GPU_POOL_VERTICES_SIZE = 4 * GiB; // 4 GB (7150 chunks allowed)
    static constexpr size_t GPU_POOL_INDICES_SIZE = GiB; // 1 GB (6710 chunks allowed)

    const GLuint MAX_VERTICES = 30000;
    const GLuint MAX_INDICES = 40000;
    
    struct Slot {
        ChunkCoords coords;
        GLuint vertexOffset, indexOffset;
        GLuint vertexCount, indexCount;
        glm::vec3 worldPos;
        bool active = false;
    };
    
    std::array<Slot, 10000> slots;
    std::unordered_map<ChunkCoords, size_t, PairHash> lookup;

    GLuint totalVertices = 0;
    GLuint totalIndices = 0;

    ChunkRenderer();

    void render(const glm::mat4& camMatrix);
    void uploadChunk(ChunkCoords coords, 
                     const std::vector<Vertex>& vertices,
                     const std::vector<GLuint>& indices);
    void deleteChunk(ChunkCoords coords);
    void resetChunks();
private:
    bool resetData;
    Shader worldShader;
    bool dirty = false;
};

#endif
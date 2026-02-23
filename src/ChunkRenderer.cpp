#include "ChunkRenderer.h"
#include "Profiler.h"

ChunkRenderer::ChunkRenderer()
    : worldShader("ResourceFiles/Shaders/world.vert", 
        "ResourceFiles/Shaders/world.frag")
{
    worldShader.setVec3Loc();
    vao.Bind();
    vbo.Bind();
    ebo.Bind();

    // --- Allocate a 200 MB pool for vertex data ---
    glBufferData(GL_ARRAY_BUFFER,
                 GPU_POOL_VERTICES_SIZE,
                 nullptr,
                 GL_DYNAMIC_DRAW);

    // --- Allocate a 200 MB pool for index data ---
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 GPU_POOL_INDICES_SIZE,
                 nullptr,
                 GL_DYNAMIC_DRAW);

    // Tell the VAO how to interpret a Vertex struct
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex),
        (void*)offsetof(Vertex, position));

    vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex),
        (void*)offsetof(Vertex, texCoord));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void ChunkRenderer::render(const glm::mat4& camMatrix)
{
    worldShader.Activate();
    vao.Bind();
    vbo.Bind();
    ebo.Bind();
    worldShader.setMat4("", camMatrix);
    
    size_t totalVertices = 0;
    size_t totalIndices = 0;
    size_t totalChunksLoaded = 0;

    // if (resetData) {
    //     totalVertices = 0;
    //     totalIndices = 0;
    //     totalChunksLoaded = 0;
    //     for (int i = 0; i < 10000; i++) {
    //         if (slots[i].active) {
    //             totalChunksLoaded++;
    //             totalVertices += slots[i].vertexCount;
    //             totalIndices += slots[i].indexCount;

    //             std::cout << "ACTUAL VERTEXES USED: " << slots[i].vertexCount << "\n";
    //             std::cout << "ACTUAL INDICES USED: " << slots[i].indexCount << "\n";

    //             std::cout << "MAX VERTEX: " << MAX_VERTICES << "\n";
    //             std::cout << "MAX INDEX: " << MAX_INDICES << "\n\n";
    //         }
    //     }
    //     // std::cout << "TOTAL WORLD MEMORY | Vertices: " <<  static_cast<size_t>(totalVertices) << "Indices: " <<  static_cast<size_t>(totalIndices)  << "\n";
    //     // std::cout << "TOTAL MEMORY RESERVED | Vertices: " << totalChunksLoaded * static_cast<size_t>(MAX_VERTICES) * SIZEOF_VERTEX << " Indices: " << totalChunksLoaded * static_cast<size_t>(MAX_INDICES) * SIZEOF_INDEX << "\n";
    //     // std::cout << "TOTAL CHUNKS LODADED: " << totalChunksLoaded << "\n";

    //     resetData = false;
    // }

    //Profiler::profile([&]() {
    for (auto& slot : slots) {
        if (!slot.active) continue;
        worldShader.setVec3(slot.worldPos);
        
        glDrawElementsBaseVertex(
            GL_TRIANGLES, 
            slot.indexCount, 
            GL_UNSIGNED_INT, 
            (void*)(slot.indexOffset * sizeof(GLuint)),
            slot.vertexOffset  // Adds this to every index
        );
    }
    //});
    vao.Unbind();
    vbo.Unbind();  // Clean unbind
    ebo.Unbind();  // Clean unbind
}

void ChunkRenderer::uploadChunk(
    ChunkCoords coords,
    const std::vector<Vertex>& vertices,
    const std::vector<GLuint>& indices)
{
    //Profiler::profile([&]() {
    // std::cout << coords.first << coords.second << "\n";
    // static int count = 0;
    // static size_t total = 0;
    // if (count >= 200){
    //     std::cout << "Vertex: " << ((float)total * 5 * 4) / count << "\n"; 
    //     count = 0;
    //     total = 0;
    // }

    // static int indexCount = 0;
    // static size_t indexTotal = 0;
    // if (indexCount >= 200){
    //     std::cout << "Index: " << ((float)indexTotal * 4) / indexCount << "\n";
    //     indexCount = 0;
    //     indexTotal = 0;
    // }

    // count++;
    // indexCount++;
    // total += vertices.size();
    // indexTotal += indices.size();

    if (vertices.size() > MAX_VERTICES) {
        //std::cout << "ERROR: " << vertices.size() << " exceeded vertices size" "\n";
        return;
    }
    if (indices.size() > MAX_INDICES) {
        //std::cout << "ERROR: " << indices.size() << " exceeded indices size" "\n";
        return;
    }
    
    auto it = lookup.find(coords);
    if (it != lookup.end()) {
        Slot& slot = slots[it->second];
        //Profiler::profile([&]() {
        vbo.Bind();
        glBufferSubData(GL_ARRAY_BUFFER,
            it->second * MAX_VERTICES * sizeof(Vertex),
            vertices.size() * sizeof(Vertex),
            vertices.data());

        ebo.Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
            it->second * MAX_INDICES * sizeof(GLuint),
            indices.size() * sizeof(GLuint),
            indices.data());

        slot.vertexCount = (GLuint)vertices.size();
        slot.indexCount = (GLuint)indices.size();
        //});
    }
    else {
        GLuint slotIndex = 0;
        for (int i = 0; i < 10000; i++) {
            if (slots[i].active == false) {
                slotIndex = i;
                break;
            }
        }
        GLuint vertexSlotStart = slotIndex * MAX_VERTICES;
        GLuint indexSlotStart = slotIndex * MAX_INDICES;
        
        vbo.Bind();
        glBufferSubData(GL_ARRAY_BUFFER,
            vertexSlotStart * sizeof(Vertex),  // Byte offset to slot start
            vertices.size() * sizeof(Vertex),  // Only actual data size
            vertices.data());
        
        ebo.Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
            indexSlotStart * sizeof(GLuint),
            indices.size() * sizeof(GLuint),
            indices.data());
        
        lookup[coords] = slotIndex;
        slots[slotIndex] = Slot{
            coords,
            vertexSlotStart,              // Vertex INDEX (not bytes)
            indexSlotStart,               // Index INDEX (not bytes)
            (GLuint)vertices.size(),      // Actual data - ignores garbage
            (GLuint)indices.size(),       // Actual data - ignores garbage
            glm::vec3(coords.first * 16, 0, coords.second * 16),
            true
        };

        totalVertices += MAX_VERTICES;
        totalIndices += MAX_INDICES;
    }
    resetData = true;
    //});
    //std::cout << "SIZE: " << lookup.size() << "\n";
}

void ChunkRenderer::deleteChunk(ChunkCoords coords)
{
    auto it = lookup.find(coords);
    if (it != lookup.end()) {
        slots[it->second].active = false;
        lookup.erase(coords);
    }
}

void ChunkRenderer::resetChunks()
{
    //slots.clear();
    lookup.clear();
    totalVertices = 0;
    totalIndices = 0;
}
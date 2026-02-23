#define GLM_ENABLE_EXPERIMENTAL
#ifndef MOB_H
#define MOB_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

#include <filesystem>
#include <deque>
#include <queue>
#include <unordered_set>
#include <vector>
#include <string>
#include <cmath>
#include <memory>

#include "PhysicsEngine.h" 

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

class Camera;

namespace std {
    template<>
    struct hash<glm::vec3> {
        size_t operator()(const glm::vec3& v) const {
            size_t h1 = hash<float>()(v.x);
            size_t h2 = hash<float>()(v.y);
            size_t h3 = hash<float>()(v.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}
// Equality operator for glm::vec3 (needed for unordered_set)
inline bool operator==(const glm::vec3& a, const glm::vec3& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}


enum class FaceDirection {
    TOP, BOTTOM, NORTH, SOUTH, EAST, WEST
};

enum class Direction {
    NONE,
    NORTH, NORTHEAST, EAST, SOUTHEAST,
    SOUTH, SOUTHWEST, WEST, NORTHWEST,
};

struct mobVertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    int partID;
};

struct Node {
    glm::vec3 dirOffset;
    glm::vec3 pos;
    bool jump = false;

    Node (glm::vec3 newPos, glm::vec3 newDir, bool j = false)
        : dirOffset(newDir), 
        pos(newPos),
        jump(j) {}
};

struct PathNode {
    glm::vec3 pos;
    glm::vec3 dirOffset;
    float g;
    float h;
    float f;
    PathNode* parent;
    bool jump = false;

    PathNode(glm::vec3 p, float gCost, float hCost, PathNode* par = nullptr, bool j = false)
        : pos(p), g(gCost), h(hCost), f(gCost + hCost), parent(par), jump(j) {}
};

class Mob {
        
};

#endif
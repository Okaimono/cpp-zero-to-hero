#define GLM_ENABLE_EXPERIMENTAL
#ifndef CREEPER_H
#define CREEPER_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Mob.h"

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

struct CreeperMesh {
public:
    // Change from static objects to static pointers
    static VAO* creeperVAO;
    static VBO* creeperVBO;
    static EBO* creeperEBO;

    static size_t headIndicesSize, bodyIndicesSize, legIndicesSize;
    static size_t totalIndexCount;
    static unsigned int headOffset, bodyOffset, legOffset;

    static bool initialized;

    static CreeperMesh& get() {
        static CreeperMesh instance;
        return instance;
    }
    
private:
    std::vector<mobVertex> vertices;
    std::vector<GLuint> indices;

    CreeperMesh() {
        if (!initialized) {
            // Create OpenGL objects only when first needed
            creeperVAO = new VAO();
            creeperVBO = new VBO();
            creeperEBO = new EBO();
            
            buildMesh();
            initialized = true;
        }
    }
    
    void buildMesh();
    void createHead();
    void createBody();
    void createLeg();
};

class Creeper : public Mob {
    public:
        Shader mobShader;

        Shader* shader = nullptr;
        Camera* camera = nullptr;

        glm::vec3 Position;
        glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);

        glm::vec3 headOrientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 realHeadOrientation = glm::vec3(0.0f, 0.0f, -1.0f);

        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 Velocity = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3& playerPosRef;

        std::unique_ptr<PhysicsEngine> creeperPhysics;

        Creeper(Shader* s, Camera* cam, glm::vec3 spawnPos);
        ~Creeper();

        void gameTick(World* world);
        void getOrientation();
        void renderCreeper(const glm::mat4& camMatrix);
        void renderLeg(const glm::mat4& camMatrix, const glm::vec3& legPos, const glm::mat4& orientParts, float swingAngle);
        void matrix();
        void renderHead();
        void explosion();
        void createHead();
        void createBody();
        void createLeg();
        bool destroyCreeper();

    private:
        GLint loc;
        static constexpr float WALK_SPEED = 4.0f;
        static constexpr float dt         = 1.0f / 120.0f;

        inline static const float accel = [](){
            constexpr float friction    = 0.85f;
            constexpr float targetSpeed = WALK_SPEED;
            constexpr float rampTime    = 0.35f;
            float k = std::log(friction) / dt;
            return (-k * targetSpeed) / (1.0f - std::exp(rampTime * k));
        }();   // ← these () are required!

        std::deque<Node> pathToFollow;

        glm::vec3 lastPathTarget = glm::vec3(0);    
        const float MIN_UPDATE_TIME = 1.0f;     // Min 0.5s between updates
        const float MIN_DISTANCE_MOVED = 3.0f;  // Or player moved 3+ block
        float pathUpdateTimer = 0.0f;

        float mobAngleX = 0.0f;
        float theta = 0.0f;
        float countdown = 1.5f;

        float HP;
        float maxHP;
        
        bool walking = true;
        bool destroy = false;
        bool showDebug = false;

        float countdownRadius = 3.0f;
        size_t radius = 4;

        void getNextNode();
        void createPath(World* world);
        void aStar(World* world);
        float octileDistance(glm::vec3 a, glm::vec3 b);
        void renderDebugCube();  
        void getHeadOrientation();

        glm::mat4 getLegMatrix(const glm::vec3& legPos, const glm::mat4& orientParts, float swingAngle);

        void jump();

        struct PathNode {
            glm::vec3 pos;
            float g; // Cost from start
            float h; // Heuristic to goal
            float f; // Total (g + h)
            PathNode* parent;
            bool jump;
            
            // Default constructor (required for std::vector)
            PathNode() 
                : pos(0.0f, 0.0f, 0.0f), 
                g(0.0f), 
                h(0.0f), 
                f(0.0f), 
                parent(nullptr), 
                jump(false) {}
            
            // Parameterized constructor
            PathNode(glm::vec3 p, float gCost, float hCost, PathNode* par = nullptr, bool j = false)
                : pos(p), 
                g(gCost), 
                h(hCost), 
                f(gCost + hCost), 
                parent(par), 
                jump(j) {}
        };

        struct NodePool {
            std::vector<PathNode> nodes;
            size_t index = 0;
            
            NodePool() : nodes(10000) {}
            
            PathNode* allocate(glm::vec3 pos, float g, float h, PathNode* parent = nullptr, bool jump = false) {
                if (index >= nodes.size()) return nullptr;
                PathNode* node = &nodes[index++];
                node->pos = pos;
                node->g = g;
                node->h = h;
                node->f = g + h;
                node->parent = parent;
                node->jump = jump;
                return node;
            }
            
            void reset() { index = 0; }
        };
        
        NodePool nodePool;
        
        // Spatial hash for closed list (much faster than unordered_set)
        std::unordered_set<int64_t> closedListHash;
        
        static int64_t encodePos(const glm::vec3& pos) {
            int32_t x = (int32_t)std::floor(pos.x);
            int32_t y = (int32_t)std::floor(pos.y);
            int32_t z = (int32_t)std::floor(pos.z);
            
            // Spatial hash using prime numbers
            return (int64_t)x * 73856093LL ^ 
                (int64_t)y * 19349663LL ^ 
                (int64_t)z * 83492791LL;
        }

                
        struct OpenList {
            PathNode** data = nullptr; // array of PathNode* (not owning)
            int size = 0;
            int capacity = 0;

            OpenList(PathNode** buf, int cap)
                : data(buf), size(0), capacity(cap) {}

            inline bool empty() const {
                return size == 0;
            }

            inline void clear() {
                size = 0;
            }

            inline void push(PathNode* node) {
                // assume: size < capacity
                int i = size++;
                data[i] = node;

                // sift up
                while (i > 0) {
                    int parent = (i - 1) >> 1;
                    if (data[parent]->f <= data[i]->f) break;
                    std::swap(data[parent], data[i]);
                    i = parent;
                }
            }

            inline PathNode* top() const {
                return data[0];
            }

            inline void pop() {
                // move last to root and sift down
                data[0] = data[--size];

                int i = 0;
                while (true) {
                    int left  = (i << 1) + 1;
                    if (left >= size) break;

                    int right = left + 1;
                    int smallest = left;

                    if (right < size && data[right]->f < data[left]->f) {
                        smallest = right;
                    }

                    if (data[i]->f <= data[smallest]->f) break;

                    std::swap(data[i], data[smallest]);
                    i = smallest;
                }
            }
        };
};

#endif
#include "Creeper.h"
#include "Camera.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace fs = std::filesystem;


Creeper::Creeper(Shader* s, Camera* cam, glm::vec3 spawnPos)
    : shader(cam->getShaderPointer()),
    mobShader("ResourceFiles/Shaders/mob.vert", "ResourceFiles/Shaders/mob.frag"),
    camera(cam),
    Position(spawnPos),
    creeperPhysics(std::make_unique<PhysicsEngine>(Position, Orientation, Velocity, 1.61f * 0.85f)), 
    playerPosRef(cam->getPositionRef())
{
    CreeperMesh::get();
    loc = glGetUniformLocation(mobShader.ID, "uPartMatrices");
    mobShader.Activate();
}

Creeper::~Creeper()
{
    PhysicsEngine::removeEngine(creeperPhysics->getID());
}

void Creeper::gameTick(World* world)
{
    static glm::vec3 staticPos = glm::floor(camera->getPosition());
    glm::vec3 mobBlockPos = glm::floor(Position);
    const float distanceToPlayer = glm::length(camera->getPosition() - Position); 
    bool isOnGround = creeperPhysics->isOnGround();

    walking = distanceToPlayer > 2.0f && distanceToPlayer < 25.0f;
    if (!walking) {return;}
    if (distanceToPlayer < 2.0f) countdown -= 1.0f * dt;

    if (countdown <= 0) {
        explosion();
        countdown = 1.0f;
        return;
    }

    if (walking) {
        pathUpdateTimer += dt;
        
        glm::vec3 currentPlayerPos = glm::floor(camera->getPosition());
        float distMoved = glm::distance(currentPlayerPos, lastPathTarget);
        
        bool shouldUpdate = pathToFollow.empty() 
                        || (distMoved >= MIN_DISTANCE_MOVED);
        
        if (shouldUpdate & pathUpdateTimer >= MIN_UPDATE_TIME) {
            aStar(world);
            lastPathTarget = currentPlayerPos;
            pathUpdateTimer = 0.0f;
        }
        if (pathToFollow.empty()) return;

        Node& moveTo = pathToFollow.front();
        if (walking) {
            glm::vec3 centerPos = moveTo.pos;
            glm::vec3 dirToCenter = glm::vec3(centerPos.x + 0.5f, centerPos.y + 0.5f, centerPos.z + 0.5f) - Position;

            float distToCenter = glm::length(dirToCenter);
            if (distToCenter < 0.3f) {
                getNextNode();
            } 
            else {
                if (moveTo.jump && isOnGround && Position.y <= moveTo.pos.y ) {
                    jump();
                }
                glm::vec3 newDirection = glm::normalize(dirToCenter);
                Orientation = glm::normalize(glm::mix(Orientation, newDirection, 0.15f));
                glm::vec3 accelerate = Orientation * accel;

                Velocity.x += (accelerate.x) * dt;
                Velocity.z += (accelerate.z) * dt;
            }
        }
    }
    else if (!pathToFollow.empty())
    {
        pathToFollow.clear();
    }
}

void Creeper::getNextNode()
{
    if (!pathToFollow.empty()) {
        pathToFollow.pop_front();
    }
}
static const glm::vec3 NEIGHBOR_OFFSETS[8] = {
    glm::vec3(1, 0, 0),     // East
    glm::vec3(-1, 0, 0),    // West
    glm::vec3(0, 0, -1),    // North
    glm::vec3(0, 0, 1),     // South
    glm::vec3(1, 0, -1),    // Northeast
    glm::vec3(1, 0, 1),     // Southeast
    glm::vec3(-1, 0, -1),   // Northwest
    glm::vec3(-1, 0, 1)     // Southwest
};

static const float DIAGONAL_COST = 1.414f;
static const float STRAIGHT_COST = 1.0f;
static const float FALL_DIAGONAL = 2.828f;
static const float FALL_STRAIGHT = 2.0f;

void Creeper::aStar(World* world)
{
    pathToFollow.clear();
    nodePool.reset();
    closedListHash.clear();
    
    glm::vec3 start = glm::floor(Position);
    glm::vec3 goal = glm::floor(camera->getPosition());

    // Find ground level for goal
    while (goal.y > 0) {
        const ItemID* block = world->getBlockAtCoords(goal);
        if (*block != ItemID::NONE) break;  
        goal.y--;
    }
    
    // Lambda for priority queue
    auto cmp = [](PathNode* a, PathNode* b) { 
        return a->f > b->f;
    };

    static PathNode* openBuffer[10000];   // Enough for your pool size
    OpenList openList(openBuffer, 10000);
    
    // Allocate start node from pool
    float startH = octileDistance(start, goal);
    PathNode* startNode = nodePool.allocate(start, 0, startH);
    if (!startNode) return; // Pool exhausted
    
    openList.push(startNode);
    
    PathNode* goalNode = nullptr;
    PathNode* bestEffortNode = nullptr;
    float bestDistanceToGoal = FLT_MAX;

    const float maxRadius = 30.0f;
    const float maxRadiusSq = maxRadius * maxRadius; // Use squared distance (avoid sqrt)
    const int maxIterations = 10000;
    
    int iterations = 0;
    while (!openList.empty() && iterations < maxIterations) {
        iterations++;
        
        PathNode* current = openList.top();
        openList.pop();
        
        // Cache distance calculation (was computed twice before)
        float distToGoalSq = 
            (current->pos.x - goal.x) * (current->pos.x - goal.x) +
            (current->pos.y - goal.y) * (current->pos.y - goal.y) +
            (current->pos.z - goal.z) * (current->pos.z - goal.z);
        
        // Track best effort node
        if (distToGoalSq < bestDistanceToGoal) {
            bestDistanceToGoal = distToGoalSq;
            bestEffortNode = current;
        }
        
        // Goal check (use squared distance)
        if (distToGoalSq < 1.0f) {
            goalNode = current;
            break;
        }
        
        // Add to closed list using spatial hash
        closedListHash.insert(encodePos(current->pos));
        
        // Check 8 neighbors using precomputed offsets
        for (int i = 0; i < 8; i++) {
            glm::vec3 neighborPos = current->pos + NEIGHBOR_OFFSETS[i];
            
            // Early radius check (squared distance, no sqrt)
            float dx = neighborPos.x - goal.x;
            float dz = neighborPos.z - goal.z;
            if (dx*dx + dz*dz > maxRadiusSq) continue;
            
            // Check if already visited
            int64_t neighborHash = encodePos(neighborPos);
            if (closedListHash.count(neighborHash) > 0) continue;
            
            // Get blocks for collision detection
            const ItemID* blocks[5];
            for (int j = -3; j < 2; j++) {
                glm::vec3 checkPos = neighborPos;
                checkPos.y += j;
                blocks[j + 3] = world->getBlockAtCoords(checkPos);
            }
            
            // Check movement type
            bool isWalk = (*blocks[3] == ItemID::NONE && 
                          *blocks[2] == ItemID::NONE && 
                          *blocks[1] != ItemID::NONE);
            
            bool isFall = (*blocks[3] == ItemID::NONE && 
                          *blocks[2] == ItemID::NONE &&
                          *blocks[1] == ItemID::NONE && 
                          *blocks[0] != ItemID::NONE);
            
            bool isJump = (*blocks[4] == ItemID::NONE &&
                          *blocks[3] == ItemID::NONE &&
                          *blocks[2] != ItemID::NONE);
            
            if (!isWalk && !isFall && !isJump) continue;
            
            // Calculate cost
            bool isDiagonal = (i >= 4);
            float moveCost;
            bool jump = false;
            
            if (isWalk) {
                moveCost = isDiagonal ? DIAGONAL_COST : STRAIGHT_COST;
            } else if (isFall) {
                moveCost = isDiagonal ? FALL_DIAGONAL : FALL_STRAIGHT;
                neighborPos.y -= 1;
            } else { // isJump
                if (isDiagonal) continue; // No diagonal jumps
                moveCost = 1.5f; // Jump cost
                jump = true;
                neighborPos.y += 1;
            }
            
            // Create neighbor from pool
            float gCost = current->g + moveCost;
            float hCost = octileDistance(neighborPos, goal);
            
            PathNode* neighbor = nodePool.allocate(neighborPos, gCost, hCost, current, jump);
            if (!neighbor) break; // Pool exhausted
            
            openList.push(neighbor);
        }
    }
    
    // Reconstruct path
    PathNode* finalNode = goalNode ? goalNode : (bestEffortNode ? bestEffortNode : startNode);
    
    PathNode* current = finalNode;
    while (current->parent != nullptr) {
        glm::vec3 dir = current->pos - current->parent->pos;
        pathToFollow.push_front(Node(current->pos, dir, current->jump));
        current = current->parent;
    }
}

// Optimized octile distance (inline for speed)
inline float Creeper::octileDistance(glm::vec3 a, glm::vec3 b) {
    float dx = std::abs(a.x - b.x);
    float dy = std::abs(a.y - b.y);
    float dz = std::abs(a.z - b.z);
    
    // Sort: dx >= dy >= dz
    if (dx < dy) std::swap(dx, dy);
    if (dy < dz) std::swap(dy, dz);
    if (dx < dy) std::swap(dx, dy);
    
    return DIAGONAL_COST * dz + DIAGONAL_COST * (dy - dz) + (dx - dy);
}

void Creeper::getOrientation()
{
    glm::vec3 playerPos = camera->getPosition();
    glm::vec3 vecToPlayer = playerPos - Position;
    vecToPlayer.y = 0.0f;
}

void Creeper::explosion()
{
    camera->explosion(Position, radius);
    destroy = true;
}

void Creeper::jump() {
// 	// keeping it here for performance, will refractor later
	float jumpSpeed = 8.0f; // ACTUAL JUMP SPEED DONT REPLACE
	Velocity.y += jumpSpeed;
}

bool Creeper::destroyCreeper()
{
    return destroy;
}

void Creeper::getHeadOrientation()
{
    glm::vec3 orientToPlayer = playerPosRef - Position;
    if (glm::length(orientToPlayer) < 0.1f) {
        orientToPlayer = glm::vec3(0.0f, 0.0f, -1.0f);
    }
    orientToPlayer = glm::normalize(orientToPlayer);
    headOrientation = orientToPlayer;
}

static constexpr float SCALE_LEN = 0.85f;
static constexpr float PIXEL_LEN = (1.0f / 16.0f) * SCALE_LEN;
static constexpr float TORSO_HEIGHT = 12.0f * PIXEL_LEN;
static constexpr float TORSO_WIDTH  = 8.0f  * PIXEL_LEN;
static constexpr float TORSO_DEPTH  = 4.0f  * PIXEL_LEN;

static constexpr float HEAD_HEIGHT = 8.0f  * PIXEL_LEN;
static constexpr float HEAD_WIDTH  = 8.0f  * PIXEL_LEN;
static constexpr float HEAD_DEPTH  = 8.0f  * PIXEL_LEN;

static constexpr float LEG_HEIGHT = 6.0f  * PIXEL_LEN;
static constexpr float LEG_WIDTH  = 4.0f  * PIXEL_LEN;
static constexpr float LEG_DEPTH  = 4.0f  * PIXEL_LEN;

static constexpr float moveLegX = TORSO_WIDTH / 2.0f - LEG_WIDTH / 2.0f;
static constexpr float moveLegY = TORSO_HEIGHT / 2.0f + LEG_HEIGHT / 2.0f;
static constexpr float moveLegZ = TORSO_DEPTH / 2.0f + LEG_DEPTH / 2.0f;

static constexpr float GLM_PI = glm::pi<float>();

static const glm::mat4 SCALE_SIZE = glm::scale(glm::mat4(1.0f), glm::vec3(0.85f));
static constexpr float LEG_HALF_HEIGHT = (6.0f / 2.0f) * (1.0f / 16.0f);
static const glm::vec3 X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
static const glm::vec3 PIVOT_OFFSET = glm::vec3(0.0f, LEG_HALF_HEIGHT, 0.0f);
static const glm::vec3 INV_PIVOT_OFFSET = -PIVOT_OFFSET;
static const glm::mat4 WALK_ANIMATION = glm::translate(glm::mat4(1.0f), PIVOT_OFFSET);
static const glm::mat4 MOVE_CENTER = glm::translate(glm::mat4(1.0f), INV_PIVOT_OFFSET);

static const glm::vec3 LEG_OFFSETS[4] = {
    glm::vec3(-moveLegX, -moveLegY, -moveLegZ),
    glm::vec3(moveLegX, -moveLegY, -moveLegZ),
    glm::vec3(-moveLegX, -moveLegY, moveLegZ),   // Back left
    glm::vec3(moveLegX, -moveLegY, moveLegZ)     // Back right
};

void Creeper::renderCreeper(const glm::mat4& camMatrix)
{
    auto& mesh = CreeperMesh::get();

    glDisable(GL_CULL_FACE);  // Add this temporarily
    mobShader.Activate();
    glm::vec3 D = glm::vec3(0.0f, 0.0f, -1.0f);
    float dot = glm::dot(D, Orientation);
    float det = D.x * Orientation.z  - D.z * Orientation.x;
    theta = atan2(det, dot);
    glm::mat4 orientParts = glm::rotate(SCALE_SIZE, -(float)theta, glm::vec3(0.0f, 1.0f, 0.0f));

    // RENDER HEAD
    glm::mat4 headModel = glm::translate(glm::mat4(1.0f), Position);

    getHeadOrientation(); 
    bool shouldRotate = glm::length(headOrientation - realHeadOrientation) > 0.1;
    
    if (shouldRotate) {
        realHeadOrientation = glm::normalize(glm::slerp(realHeadOrientation, headOrientation, 0.0375f));
    }

    glm::vec3 forward = realHeadOrientation;
    glm::vec3 up = glm::vec3(0,1,0);

    if (fabs(glm::dot(forward, up)) > 0.99f)
        up = glm::vec3(0,0,1);

    glm::vec3 right = glm::normalize(glm::cross(up, forward));
    glm::vec3 newUp = glm::cross(forward, right);

    glm::mat4 headRot(1.0f);
    headRot[0] = glm::vec4(right,   0);
    headRot[1] = glm::vec4(newUp,   0);
    headRot[2] = glm::vec4(-forward, 0);
    // FINISH HEAD MATRIX


    // CALCULATE BODY MATRICIES
    const glm::vec3 bodyOffset = glm::vec3(0.0f, -(HEAD_HEIGHT / 2.0f) - (TORSO_HEIGHT / 2.0f), 0.0f);
    const glm::vec3 bodyPos = Position + bodyOffset;


    // RENDER LEGS
    const glm::mat3 rotateOffset = glm::rotate(glm::mat4(1.0f), -(float)theta, glm::vec3(0, 1, 0));
    glm::vec3 legPos[4];
    for (int i = 0; i < 4; i++) {
        legPos[i] = bodyPos + rotateOffset * LEG_OFFSETS[i];
    }

    mobAngleX += 0.02f;
    float rotate = 0.22f * GLM_PI * sinf(mobAngleX);

    // Precompute walk animations
    glm::mat4 walkPos = WALK_ANIMATION * glm::rotate(glm::mat4(1.0f), rotate, X_AXIS) * MOVE_CENTER;
    glm::mat4 walkNeg = WALK_ANIMATION * glm::rotate(glm::mat4(1.0f), -rotate, X_AXIS) * MOVE_CENTER;

    // Create 4 final transformation matrices

    glm::mat4 partMatrices[6] = {
        camMatrix * glm::translate(glm::mat4(1.0f), Position) * SCALE_SIZE * headRot,
        camMatrix * glm::translate(glm::mat4(1.0f), bodyPos) * orientParts,
        camMatrix * glm::translate(glm::mat4(1.0f), legPos[0]) * orientParts * walkPos,
        camMatrix * glm::translate(glm::mat4(1.0f), legPos[1]) * orientParts * walkNeg,
        camMatrix * glm::translate(glm::mat4(1.0f), legPos[2]) * orientParts * walkNeg,
        camMatrix * glm::translate(glm::mat4(1.0f), legPos[3]) * orientParts * walkPos
    };

    // Upload matrices to GPU
    glUniformMatrix4fv(loc, 6, GL_FALSE, glm::value_ptr(partMatrices[0]));


    // Draw
    mesh.creeperVAO->Bind();
    glDrawElements(GL_TRIANGLES, mesh.totalIndexCount, GL_UNSIGNED_INT, 0);

    if(showDebug)
    {
        renderDebugCube();
    }
}

void Creeper::renderDebugCube() 
{
    // Cam matrix
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 camMatrix = projection * view;
    auto& mesh = CreeperMesh::get();    
    /*
    for (const auto& node : pathToFollow) {
        glm::vec3 debugPos = node.pos + glm::vec3(0.5f, 0.5f, 0.5f);
        glm::mat4 debugModel = glm::translate(glm::mat4(0.1f), debugPos);
        debugModel = glm::scale(debugModel, glm::vec3(0.3f));  // Make it small

        mesh.headVAO->Bind();
        mobShader.setMat4("camMatrix", camMatrix * debugModel);
        glDrawElements(GL_TRIANGLES, mesh.headIndicesSize, GL_UNSIGNED_INT, 0);
        mesh.headVAO->Unbind();
    }
    */
}

VAO*   CreeperMesh::creeperVAO = nullptr;
VBO*   CreeperMesh::creeperVBO = nullptr;
EBO*   CreeperMesh::creeperEBO = nullptr;

size_t CreeperMesh::headIndicesSize = 0;
size_t CreeperMesh::bodyIndicesSize = 0;
size_t CreeperMesh::legIndicesSize  = 0;
size_t CreeperMesh::totalIndexCount = 0;

unsigned int CreeperMesh::headOffset = 0;
unsigned int CreeperMesh::bodyOffset = 0;
unsigned int CreeperMesh::legOffset  = 0;

bool CreeperMesh::initialized = false;


// =======================
//   BUILD MESH
// =======================
void CreeperMesh::buildMesh()
{
    createHead();
    createBody();
    createLeg();

    totalIndexCount = indices.size();

    creeperVAO->Bind();
    creeperVBO->Bind();
    creeperEBO->Bind();

    // Upload unified mesh
    creeperVBO->Upload(vertices.data(), vertices.size() * sizeof(mobVertex));
    creeperEBO->Upload(indices.data(), indices.size() * sizeof(GLuint));

    // vertex attribs: pos, uv, partID
    creeperVAO->LinkAttrib(*creeperVBO, 0, 3, GL_FLOAT, sizeof(mobVertex), (void*)0);
    
    creeperVAO->LinkAttrib(*creeperVBO, 1, 2, GL_FLOAT, sizeof(mobVertex), (void*)(3 * sizeof(float)));
    creeperVAO->LinkAttrib(*creeperVBO, 2, 1, GL_FLOAT, sizeof(mobVertex), (void*)(5 * sizeof(float)));

    glBindVertexArray(0);
}

void CreeperMesh::createHead()
{
    const float headHeightPixels = 8.0f;
    const float headWidthPixels = 8.0f;
    const float headDepthPixels = 8.0f;

    const float headHeight = headHeightPixels / 16.0f;
    const float headWidth = headWidthPixels / 16.0f;
    const float headDepth = headDepthPixels / 16.0f;

    const float halfHeight = headHeight / 2.0f;
    const float halfWidth = headWidth / 2.0f;
    const float halfDepth = headDepth / 2.0f;

    float topLeftX = 8.0f, topLeftY = 8.0f;
    float texWidth = 64.0f, texHeight = 32.0f;
    float faceSize = 8.0f;

    float x = 0;
    float y = 0;
    float z = 0;

    const int PART_ID = 0;  // HEAD = 0

    topLeftX = 16.0f;
    topLeftY = 8.0f;
    faceSize = 8.0f;

    // Top face
    mobVertex vert1 = { {x - halfWidth, y + halfHeight, z - halfDepth},  {topLeftX / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert2 = { {x + halfWidth, y + halfHeight, z - halfDepth},  {(topLeftX + faceSize) / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert3 = { {x + halfWidth, y + halfHeight, z + halfDepth},  {(topLeftX + faceSize) / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert4 = { {x - halfWidth, y + halfHeight, z + halfDepth},  {topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};

    topLeftX = 16.0f;
    topLeftY = 8.0f;
    faceSize = 8.0f;

    // Bottom face
    mobVertex vert5 = { {x - halfWidth, y - halfHeight, z - halfDepth},  {topLeftX / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert6 = { {x + halfWidth, y - halfHeight, z - halfDepth},  {(topLeftX + faceSize) / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert7 = { {x + halfWidth, y - halfHeight, z + halfDepth},  {(topLeftX + faceSize) / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert8 = { {x - halfWidth, y - halfHeight, z + halfDepth},  {topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};

    topLeftX = 8.0f;
    topLeftY = 8.0f;
    faceSize = 8.0f;

    // North face (face)
    mobVertex vert9 = { {x - halfWidth, y - halfHeight, z - halfDepth},  {topLeftX / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert10 = { {x + halfWidth, y - halfHeight, z - halfDepth},  {(topLeftX + faceSize) / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert11 = { {x + halfWidth, y + halfHeight, z - halfDepth},  {(topLeftX + faceSize) / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert12 = { {x - halfWidth, y + halfHeight, z - halfDepth},  {topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};

    topLeftX = 16.0f;
    topLeftY = 8.0f;
    faceSize = 8.0f;
    
    // South face
    mobVertex vert13 = { {x + halfWidth, y - halfHeight, z + halfDepth},  {topLeftX / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert14 = { {x - halfWidth, y - halfHeight, z + halfDepth},  {(topLeftX + faceSize) / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert15 = { {x - halfWidth, y + halfHeight, z + halfDepth},  {(topLeftX + faceSize) / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert16 = { {x + halfWidth, y + halfHeight, z + halfDepth},  {topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};

    topLeftX = 16.0f;
    topLeftY = 8.0f;
    faceSize = 8.0f;
    
    // East face
    mobVertex vert17 = { {x + halfWidth, y - halfHeight, z + halfDepth},  {topLeftX / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert18 = { {x + halfWidth, y - halfHeight, z - halfDepth},  {(topLeftX + faceSize) / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert19 = { {x + halfWidth, y + halfHeight, z - halfDepth},  {(topLeftX + faceSize) / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert20 = { {x + halfWidth, y + halfHeight, z + halfDepth},  {topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};

    topLeftX = 16.0f;
    topLeftY = 8.0f;
    faceSize = 8.0f;
    
    // West face
    mobVertex vert21 = { {x - halfWidth, y - halfHeight, z - halfDepth},  {topLeftX / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert22 = { {x - halfWidth, y - halfHeight, z + halfDepth},  {(topLeftX + faceSize) / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert23 = { {x - halfWidth, y + halfHeight, z + halfDepth},  {(topLeftX + faceSize) / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert24 = { {x - halfWidth, y + halfHeight, z - halfDepth},  {topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};

    // ... rest of function unchanged ...
    headOffset = indices.size();
    unsigned int base = vertices.size();

    vertices.insert(vertices.end(), {
        vert1, vert2, vert3, vert4,      // Top face: 0-3
        vert5, vert6, vert7, vert8,      // Bottom face: 4-7
        vert9, vert10, vert11, vert12,   // North face: 8-11
        vert13, vert14, vert15, vert16,  // South face: 12-15
        vert17, vert18, vert19, vert20,  // East face: 16-19
        vert21, vert22, vert23, vert24   // West face: 20-23
    });

    indices.insert(indices.end(), {
        // Top face (0-3)
        base + 0, base + 1, base + 2,
        base + 2, base + 3, base + 0,
        
        // Bottom face (4-7)
        base + 4, base + 5, base + 6,
        base + 6, base + 7, base + 4,
        
        // North face (8-11)
        base + 8, base + 9, base + 10,
        base + 10, base + 11, base + 8,
        
        // South face (12-15)
        base + 12, base + 13, base + 14,
        base + 14, base + 15, base + 12,
        
        // East face (16-19)
        base + 16, base + 17, base + 18,
        base + 18, base + 19, base + 16,
        
        // West face (20-23)
        base + 20, base + 21, base + 22,
        base + 22, base + 23, base + 20
    });
    headIndicesSize = indices.size();
}

void CreeperMesh::createBody()
{

    // Body dimensions in pixels
    const float bodyHeightPixels = 12.0f;
    const float bodyWidthPixels = 8.0f;
    const float bodyDepthPixels = 4.0f;

    // Convert to world units (1 pixel = 1/16 of a unit)
    const float bodyHeight = bodyHeightPixels / 16.0f;
    const float bodyWidth = bodyWidthPixels / 16.0f;
    const float bodyDepth = bodyDepthPixels / 16.0f;

    // Half dimensions for vertex positioning
    const float halfHeight = bodyHeight / 2.0f;
    const float halfWidth = bodyWidth / 2.0f;
    const float halfDepth = bodyDepth / 2.0f;

    float topLeftX = 16.0f, topLeftY = 8.0f;
    float texWidth = 64.0f, texHeight = 32.0f;
    float faceHeight = 8.0f;
    float faceWidth = 12.0f;

    float x = 0;
    float y = 0;
    float z = 0;

    const int PART_ID = 1;

    float faceSize = 8.0f;

    // Top face
    mobVertex vert1 = { {x - halfWidth, y + halfHeight, z - halfDepth},  {topLeftX / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert2 = { {x + halfWidth, y + halfHeight, z - halfDepth},  {(topLeftX + faceSize) / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert3 = { {x + halfWidth, y + halfHeight, z + halfDepth},  {(topLeftX + faceSize) / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert4 = { {x - halfWidth, y + halfHeight, z + halfDepth},  {topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};

    topLeftX = 16.0f;
    topLeftY = 8.0f;
    faceSize = 8.0f;

    // Bottom face
    mobVertex vert5 = { {x - halfWidth, y - halfHeight, z - halfDepth},  {topLeftX / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert6 = { {x + halfWidth, y - halfHeight, z - halfDepth},  {(topLeftX + faceSize) / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert7 = { {x + halfWidth, y - halfHeight, z + halfDepth},  {(topLeftX + faceSize) / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert8 = { {x - halfWidth, y - halfHeight, z + halfDepth},  {topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};

    topLeftX = 16.0f;
    topLeftY = 8.0f;
    faceHeight = 8.0f;
    faceWidth = 12.0f;

    // North face
    mobVertex vert9 = { {x - halfWidth, y - halfHeight, z - halfDepth},  { (topLeftX + faceWidth) / 64.0f, (topLeftY + faceHeight) / 32.0f}, PART_ID};
    mobVertex vert10 = { {x + halfWidth, y - halfHeight, z - halfDepth},  { (topLeftX + faceWidth) / 64.0f, (topLeftY) / 32.0f}, PART_ID};
    mobVertex vert11 = { {x + halfWidth, y + halfHeight, z - halfDepth},  { topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert12 = { {x - halfWidth, y + halfHeight, z - halfDepth},  { topLeftX / 64.0f, (topLeftY + faceHeight) / 32.0f}, PART_ID};

    topLeftX = 16.0f;
    topLeftY = 8.0f;
    faceSize = 8.0f;
    
    // South face
    mobVertex vert13 = { {x + halfWidth, y - halfHeight, z + halfDepth},  {topLeftX / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert14 = { {x - halfWidth, y - halfHeight, z + halfDepth},  {(topLeftX + faceSize) / 64.0f, (topLeftY + faceSize) / 32.0f}, PART_ID};
    mobVertex vert15 = { {x - halfWidth, y + halfHeight, z + halfDepth},  {(topLeftX + faceSize) / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert16 = { {x + halfWidth, y + halfHeight, z + halfDepth},  {topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};

    topLeftX = 26.0f;
    topLeftY = 17.0f;
    faceHeight = 4.0f;
    faceWidth = 8.0f;
    
    // East face
    mobVertex vert17 = { {x + halfWidth, y - halfHeight, z + halfDepth},  { (topLeftX + faceWidth) / 64.0f, (topLeftY + faceHeight) / 32.0f}, PART_ID};
    mobVertex vert18 = { {x + halfWidth, y - halfHeight, z - halfDepth},  { (topLeftX + faceWidth) / 64.0f, (topLeftY) / 32.0f}, PART_ID};
    mobVertex vert19 = { {x + halfWidth, y + halfHeight, z - halfDepth},  { topLeftX / 64.0f, topLeftY / 32.0f}, PART_ID};
    mobVertex vert20 = { {x + halfWidth, y + halfHeight, z + halfDepth},  { topLeftX / 64.0f, (topLeftY + faceHeight) / 32.0f}, PART_ID};

    topLeftX = 26.0f;
    topLeftY = 17.0f;
    faceHeight = 4.0f;
    faceWidth = 8.0f;

    // West face
    mobVertex vert21 = { {x - halfWidth, y - halfHeight, z - halfDepth}, {(topLeftX + faceWidth) / 64.0f, (topLeftY + faceHeight) / 32.0f}, PART_ID};
    mobVertex vert22 = { {x - halfWidth, y - halfHeight, z + halfDepth}, {(topLeftX + faceWidth) / 64.0f,  topLeftY / 32.0f}, PART_ID};
    mobVertex vert23 = { {x - halfWidth, y + halfHeight, z + halfDepth}, { topLeftX / 64.0f,  topLeftY / 32.0f}, PART_ID};
    mobVertex vert24 = { {x - halfWidth, y + halfHeight, z - halfDepth}, { topLeftX / 64.0f, (topLeftY + faceHeight) / 32.0f}, PART_ID};

    bodyOffset = indices.size();
    unsigned int base = vertices.size();

    vertices.insert(vertices.end(), {
        vert1, vert2, vert3, vert4,      // Top face: 0-3
        vert5, vert6, vert7, vert8,      // Bottom face: 4-7
        vert9, vert10, vert11, vert12,   // North face: 8-11
        vert13, vert14, vert15, vert16,  // South face: 12-15
        vert17, vert18, vert19, vert20,  // East face: 16-19
        vert21, vert22, vert23, vert24   // West face: 20-23
    });

    indices.insert(indices.end(), {
        // Top face (0-3)
        base + 0, base + 1, base + 2,
        base + 2, base + 3, base + 0,
        
        // Bottom face (4-7)
        base + 4, base + 5, base + 6,
        base + 6, base + 7, base + 4,
        
        // North face (8-11)
        base + 8, base + 9, base + 10,
        base + 10, base + 11, base + 8,
        
        // South face (12-15)
        base + 12, base + 13, base + 14,
        base + 14, base + 15, base + 12,
        
        // East face (16-19)
        base + 16, base + 17, base + 18,
        base + 18, base + 19, base + 16,
        
        // West face (20-23)
        base + 20, base + 21, base + 22,
        base + 22, base + 23, base + 20
    });
    bodyIndicesSize = indices.size();
}

void CreeperMesh::createLeg()
{
    // --- LEG DIMENSIONS ---
    const float legHeightPixels = 6.0f;
    const float legWidthPixels  = 4.0f;
    const float legDepthPixels  = 4.0f;

    const float legHeight = legHeightPixels / 16.0f;
    const float legWidth  = legWidthPixels  / 16.0f;
    const float legDepth  = legDepthPixels  / 16.0f;

    const float halfHeight = legHeight / 2.0f;
    const float halfWidth  = legWidth  / 2.0f;
    const float halfDepth  = legDepth  / 2.0f;

    float x = 0, y = 0, z = 0;

    // ===============================
    // BASE LEG VERTEX TEMPLATE MAKER
    // ===============================
    auto makeLegVerts = [&](int partID)
    {
        std::vector<mobVertex> v;

        float topLeftX, topLeftY, faceWidth, faceHeight;

        // -------- TOP FACE --------
        topLeftX = 4.0f; topLeftY = 16.0f;
        faceWidth = 4.0f; faceHeight = 4.0f;
        v.push_back({{x-halfWidth, y+halfHeight, z-halfDepth}, {topLeftX/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x+halfWidth, y+halfHeight, z-halfDepth}, {(topLeftX+faceWidth)/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x+halfWidth, y+halfHeight, z+halfDepth}, {(topLeftX+faceWidth)/64.0f,topLeftY/32.0f}, partID});
        v.push_back({{x-halfWidth, y+halfHeight, z+halfDepth}, {topLeftX/64.0f,topLeftY/32.0f}, partID});

        // -------- BOTTOM FACE --------
        topLeftX = 8.0f; topLeftY = 16.0f;
        v.push_back({{x-halfWidth, y-halfHeight, z-halfDepth}, {topLeftX/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x+halfWidth, y-halfHeight, z-halfDepth}, {(topLeftX+faceWidth)/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x+halfWidth, y-halfHeight, z+halfDepth}, {(topLeftX+faceWidth)/64.0f,topLeftY/32.0f}, partID});
        v.push_back({{x-halfWidth, y-halfHeight, z+halfDepth}, {topLeftX/64.0f,topLeftY/32.0f}, partID});

        // -------- NORTH FACE --------
        topLeftX = 4.0f; topLeftY = 20.0f; faceHeight = 6.0f;
        v.push_back({{x-halfWidth, y-halfHeight, z-halfDepth}, {topLeftX/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x+halfWidth, y-halfHeight, z-halfDepth}, {(topLeftX+faceWidth)/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x+halfWidth, y+halfHeight, z-halfDepth}, {(topLeftX+faceWidth)/64.0f,topLeftY/32.0f}, partID});
        v.push_back({{x-halfWidth, y+halfHeight, z-halfDepth}, {topLeftX/64.0f,topLeftY/32.0f}, partID});

        // -------- SOUTH FACE --------
        topLeftX = 12.0f; topLeftY = 20.0f;
        v.push_back({{x+halfWidth, y-halfHeight, z+halfDepth}, {topLeftX/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x-halfWidth, y-halfHeight, z+halfDepth}, {(topLeftX+faceWidth)/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x-halfWidth, y+halfHeight, z+halfDepth}, {(topLeftX+faceWidth)/64.0f,topLeftY/32.0f}, partID});
        v.push_back({{x+halfWidth, y+halfHeight, z+halfDepth}, {topLeftX/64.0f,topLeftY/32.0f}, partID});

        // -------- EAST FACE --------
        topLeftX = 0.0f; topLeftY = 20.0f;
        v.push_back({{x+halfWidth, y-halfHeight, z+halfDepth}, {topLeftX/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x+halfWidth, y-halfHeight, z-halfDepth}, {(topLeftX+faceWidth)/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x+halfWidth, y+halfHeight, z-halfDepth}, {(topLeftX+faceWidth)/64.0f,topLeftY/32.0f}, partID});
        v.push_back({{x+halfWidth, y+halfHeight, z+halfDepth}, {topLeftX/64.0f,topLeftY/32.0f}, partID});

        // -------- WEST FACE --------
        topLeftX = 8.0f; topLeftY = 20.0f;
        v.push_back({{x-halfWidth, y-halfHeight, z-halfDepth}, {topLeftX/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x-halfWidth, y-halfHeight, z+halfDepth}, {(topLeftX+faceWidth)/64.0f,(topLeftY+faceHeight)/32.0f}, partID});
        v.push_back({{x-halfWidth, y+halfHeight, z+halfDepth}, {(topLeftX+faceWidth)/64.0f,topLeftY/32.0f}, partID});
        v.push_back({{x-halfWidth, y+halfHeight, z-halfDepth}, {topLeftX/64.0f,topLeftY/32.0f}, partID});

        return v;
    };

    legOffset = indices.size();

    // =========================
    // INSERT 4 LEGS (IDs 2–5)
    // =========================
    for (int legID = 2; legID < 6; legID++)
    {
        unsigned int baseIndex = vertices.size();

        auto legVerts = makeLegVerts((float)legID);
        vertices.insert(vertices.end(), legVerts.begin(), legVerts.end());

        // Indices for 24 vertices (simple cube)
        static const GLuint cubeIdx[] = {
            0,1,2, 2,3,0,       // Top
            4,5,6, 6,7,4,       // Bottom
            8,9,10, 10,11,8,    // North
            12,13,14, 14,15,12, // South
            16,17,18, 18,19,16, // East
            20,21,22, 22,23,20  // West
        };

        for (int i = 0; i < 36; i++)
            indices.push_back(baseIndex + cubeIdx[i]);
    }

    legIndicesSize = indices.size();
}
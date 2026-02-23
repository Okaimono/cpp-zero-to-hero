#include "Cow.h"
#include "Camera.h"

#include <iostream>

namespace fs = std::filesystem;

Cow::Cow(Shader* s, Camera* cam, glm::vec3 spawnPos)
    : shader(cam->getShaderPointer()),
    texture("ResourceFiles/Texture/mobs/cow.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
    camera(cam),
    Position(spawnPos),
    creeperPhysics(std::make_unique<PhysicsEngine>(Position, Orientation, Velocity)), 
    playerPosRef(cam->getPositionRef())
{
    texture.texUnit(*shader, "tex0", 0);

    createHead();
    createBody();
    createLeg();
}

Cow::~Cow()
{
    PhysicsEngine::removeEngine(creeperPhysics->getID());
}

void Cow::gameTick(World* world)
{
    static glm::vec3 staticPos = glm::floor(camera->getPosition());
    glm::vec3 mobBlockPos = glm::floor(Position);
    const float distanceToPlayer = glm::length(camera->getPosition() - Position); 
    bool isOnGround = creeperPhysics->isOnGround();

    walking = distanceToPlayer > 2.0f && distanceToPlayer < 25.0f;

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

void Cow::getNextNode()
{
    if (!pathToFollow.empty()) {
        pathToFollow.pop_front();
    }
}

void Cow::aStar(World* world)
{
    pathToFollow.clear();
    
    glm::vec3 start = glm::floor(Position);
    glm::vec3 goal = glm::floor(camera->getPosition());

    while (goal.y > 0) {
        const ItemID* block = world->getBlockAtCoords(goal);
        if (*block != ItemID::NONE) {
            break;  // Found ground!
        }
        goal.y--;  // Keep going down
    }
    
    auto cmp = [](PathNode* a, PathNode* b) { 
        return a->f > b->f;
    };

    std::priority_queue<PathNode*, std::vector<PathNode*>, decltype(cmp)> openList(cmp);
    std::unordered_set<glm::vec3> closedList;
    std::vector<std::unique_ptr<PathNode>> allNodes;
    
    float startH = octileDistance(start, goal);  // ← Changed
    auto startNode = std::make_unique<PathNode>(start, 0, startH);
    PathNode* startPtr = startNode.get();
    openList.push(startPtr);
    allNodes.push_back(std::move(startNode));
    
    PathNode* goalNode = nullptr;
    PathNode* bestEffortNode = nullptr;
    float bestDistanceToPlayer = FLT_MAX;

    float maxRadius = 30.0f; // ← A Star Pathfinding radius limit around player
    int maxIterations = 10000; 
    int iterations = 0; 
    while (!openList.empty() && iterations < maxIterations) {
        iterations++;
        
        PathNode* current = openList.top();
        openList.pop(); 

        float distToPlayer = glm::distance(current->pos, goal);
        if (distToPlayer < bestDistanceToPlayer) {
            bestDistanceToPlayer = distToPlayer;
            bestEffortNode = current;
        }
        
        if (glm::distance(current->pos, goal) < 1.0f) {
            goalNode = current;
            break;
        }
        
        closedList.insert(current->pos);
        
        // Check 8 neighbors  // ← Fixed comment
        glm::vec3 neighbors[8] = {
            current->pos + glm::vec3(1, 0, 0),     // East
            current->pos + glm::vec3(-1, 0, 0),    // West
            current->pos + glm::vec3(0, 0, -1),    // North
            current->pos + glm::vec3(0, 0, 1),     // South
            current->pos + glm::vec3(1, 0, -1),    // Northeast
            current->pos + glm::vec3(1, 0, 1),     // Southeast
            current->pos + glm::vec3(-1, 0, -1),   // Northwest
            current->pos + glm::vec3(-1, 0, 1)     // Southwest
        };
        
        for (int i = 0; i < 8; i++) {
            glm::vec3 neighborPos = neighbors[i];
            if (glm::distance(goal, neighborPos) > maxRadius) continue;
            const ItemID* blocks[5];
            bool jump = false;

            for (int j = -3; j < 2; j++) {
                glm::vec3 checkBlockAt = neighborPos;
                checkBlockAt.y += j;
                blocks[j + 3] = world->getBlockAtCoords(checkBlockAt); 
            }
            bool isWalk = (*blocks[3] == ItemID::NONE 
                        && *blocks[2] == ItemID::NONE 
                        && *blocks[1] != ItemID::NONE);
    
            bool isFall = (*blocks[3] == ItemID::NONE 
                        && *blocks[2] == ItemID::NONE
                        && *blocks[1] == ItemID::NONE 
                        && *blocks[0] != ItemID::NONE);
            
            bool isJump = (*blocks[4] == ItemID::NONE
                        && *blocks[3] == ItemID::NONE
                        && *blocks[2] != ItemID::NONE); 

            if (!isWalk && !isFall && !isJump) {
                continue;  // Illegal move
            }
            
            bool isDiagonal = (i >= 4);
            float moveCost;

            if (isWalk) {
                moveCost = isDiagonal ? 1.414f : 1.0f;
            } else if (isFall) {
                moveCost = isDiagonal ? 2.828f : 2.0f;
                neighborPos.y -= 1;
            } else {
                jump = true;
                if (isDiagonal) continue;
                neighborPos.y += 1;
            }

            if (closedList.count(neighborPos) > 0) continue;
            
            float gCost = current->g + moveCost;
            float hCost = octileDistance(neighborPos, goal);  // ← Changed
            
            auto neighbor = std::make_unique<PathNode>(neighborPos, gCost, hCost, current, jump);
            PathNode* p_neighbor = neighbor.get();
            
            openList.push(p_neighbor);
            allNodes.push_back(std::move(neighbor));
        }
    }
    PathNode* finalNode = nullptr;

    if (goalNode != nullptr) {
        finalNode = goalNode;
    } 
    else if (bestEffortNode != nullptr) {
        finalNode = bestEffortNode;
    } 
    else {
        finalNode = startPtr;
    }
    PathNode* current = finalNode;
    while (current->parent != nullptr) {
        glm::vec3 dir = current->pos - current->parent->pos;
        bool jump = current->jump;
        pathToFollow.push_front(Node(current->pos, dir, jump));
        current = current->parent;
    }
}

// Add this function:
float Cow::octileDistance(glm::vec3 a, glm::vec3 b) {
    float dx = std::abs(a.x - b.x);
    float dy = std::abs(a.y - b.y);
    float dz = std::abs(a.z - b.z);
    
    if (dx < dy) std::swap(dx, dy);
    if (dy < dz) std::swap(dy, dz);
    if (dx < dy) std::swap(dx, dy);
    
    return 1.414f * dz + 1.414f * (dy - dz) + (dx - dy);
}

void Cow::getOrientation()
{
    glm::vec3 playerPos = camera->getPosition();
    glm::vec3 vecToPlayer = playerPos - Position;
    vecToPlayer.y = 0.0f;
}

void Cow::explosion()
{
    camera->explosion(Position, radius);
    destroy = true;
}

void Cow::jump() {
// 	// keeping it here for performance, will refractor later
	float jumpSpeed = 8.0f; // ACTUAL JUMP SPEED DONT REPLACE
	Velocity.y += jumpSpeed;
}

bool Cow::destroyCreeper()
{
    return destroy;
}

void Cow::getHeadOrientation()
{
    glm::vec3 orientToPlayer = playerPosRef - Position;
    if (glm::length(orientToPlayer) < 0.1f) {
        orientToPlayer = glm::vec3(0.0f, 0.0f, -1.0f);
    }
    orientToPlayer = glm::normalize(orientToPlayer);
    headOrientation = orientToPlayer;
}

void Cow::renderCow()
{
    const float pixelLen = 1.0f / 16.0f;

    const float torsoHeight = 10.0f * pixelLen;
    const float torsoWidth = 12.0f * pixelLen;
    const float torsoDepth = 18.0f * pixelLen;

    const float headHeight = 8.0f * pixelLen;
    const float headWidth = 8.0f * pixelLen;
    const float headDepth = 6.0f * pixelLen;

    const float legHeight = 12.0f * pixelLen;
    const float legWidth = 4.0f * pixelLen;
    const float legDepth = 4.0f * pixelLen;


    shader->Activate();
    texture.Bind();

    // Cam matrix
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 camMatrix = projection * view;

    glm::vec3 D = glm::vec3(0.0f, 0.0f, -1.0f);
    float dot = glm::dot(D, Orientation);
    float det = D.x * Orientation.z  - D.z * Orientation.x;
    theta = atan2(det, dot);
    glm::mat4 orientParts = glm::rotate(glm::mat4(1.0f), -(float)theta, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat3 rotateOffset = glm::rotate(glm::mat4(1.0f), -(float)theta, glm::vec3(0, 1, 0));



    // Render Body
    glm::vec3 bodyPos = Position;
    glm::mat4 bodyModel = glm::translate(glm::mat4(1.0f), bodyPos);

    bodyVAO.Bind();
    shader->setMat4("camMatrix", camMatrix * bodyModel * orientParts);
    glDrawElements(GL_TRIANGLES, bodyIndicesSize, GL_UNSIGNED_INT, 0);
    bodyVAO.Unbind();



    // Render head
    glm::vec3 headOffset = glm::vec3(0.0f, torsoHeight / 2.0f - (2.0f * 1.0f/16.0f), -torsoDepth / 2.0f - headDepth / 2.0f);
    headOffset = rotateOffset * headOffset;
    glm::vec3 headPos = bodyPos + headOffset;
    glm::mat4 headModel = glm::translate(glm::mat4(1.0f), headPos);

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

    glm::mat4 headTransform = headModel * headRot;

    headVAO.Bind();
    shader->setMat4("camMatrix", camMatrix * headTransform);
    glDrawElements(GL_TRIANGLES, headIndicesSize, GL_UNSIGNED_INT, 0);
    headVAO.Unbind();


    // RENDER LEGS
    float moveLegX = torsoWidth / 2.0f - legWidth / 2.0f;
    float moveLegY = torsoHeight / 2.0f + legHeight / 2.0f;
    float moveLegZ = torsoDepth / 2.0f - legDepth / 2.0f;

    // Leg offset
    glm::vec3 legOffset1 = glm::vec3(-moveLegX, -moveLegY, -moveLegZ);
    glm::vec3 legOffset2 = glm::vec3(moveLegX, -moveLegY, -moveLegZ);
    glm::vec3 legOffset3 = glm::vec3(-moveLegX, -moveLegY, moveLegZ);
    glm::vec3 legOffset4 = glm::vec3(moveLegX, -moveLegY, moveLegZ);

    // Rotate offsets towards orientation
    legOffset1 = rotateOffset * legOffset1;
    legOffset2 = rotateOffset * legOffset2;
    legOffset3 = rotateOffset * legOffset3;
    legOffset4 = rotateOffset * legOffset4;

    // Translate 
    glm::vec3 legPos1 = bodyPos + legOffset1;
    glm::vec3 legPos2 = bodyPos + legOffset2;
    glm::vec3 legPos3 = bodyPos + legOffset3;
    glm::vec3 legPos4 = bodyPos + legOffset4;

    legVAO.Bind();

  
    mobAngleX += 0.02f;
    float rotate = 0.22f * glm::pi<float>() * sinf(mobAngleX);
    renderLeg(camMatrix, legPos1, orientParts, rotate);
    renderLeg(camMatrix, legPos2, orientParts, -rotate);
    renderLeg(camMatrix, legPos3, orientParts, -rotate);
    renderLeg(camMatrix, legPos4, orientParts, rotate);

    legVAO.Unbind();

    // Render debug
    if(showDebug)
    {
        renderDebugCube();
    }
}

void Cow::renderLeg(const glm::mat4& camMatrix, const glm::vec3& legPos, const glm::mat4& orientParts, float swingAngle)
{
    float pivotY = 12.0f * (1.0f/16.0f) / 2.0f; // Pivot point offset (P)
    glm::vec3 P = glm::vec3(0.0f, pivotY, 0.0f);
    glm::mat4 walkAnimation = glm::translate(glm::mat4(1.0f), P);
    if (walking) {
        walkAnimation = glm::rotate(walkAnimation, swingAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    }
    walkAnimation = glm::translate(walkAnimation, -P);

    glm::mat4 legModel = glm::translate(glm::mat4(1.0f), legPos);
    legModel = legModel * orientParts;
    legModel = legModel * walkAnimation;

    shader->setMat4("camMatrix", camMatrix * legModel);
    glDrawElements(GL_TRIANGLES, legIndicesSize, GL_UNSIGNED_INT, 0);
}

void Cow::renderDebugCube()
{
    // Cam matrix
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 camMatrix = projection * view;

    for (const auto& node : pathToFollow) {
        glm::vec3 debugPos = node.pos + glm::vec3(0.5f, 0.5f, 0.5f);
        glm::mat4 debugModel = glm::translate(glm::mat4(0.1f), debugPos);
        debugModel = glm::scale(debugModel, glm::vec3(0.3f));  // Make it small

        headVAO.Bind();
        shader->setMat4("camMatrix", camMatrix * debugModel);
        glDrawElements(GL_TRIANGLES, headIndicesSize, GL_UNSIGNED_INT, 0);
        headVAO.Unbind();
    }
}

std::vector<glm::vec3> Cow::getVertices(float sideHeight, float sideWidth, float sideDepth, FaceDirection dir)
{
    glm::vec3 vert1;
    glm::vec3 vert2;
    glm::vec3 vert3;
    glm::vec3 vert4;

    // Pixels per in game block
    const float blockPixels = 16.0f;

    // Game dimensions of the size
    const float height = sideHeight / blockPixels; 
    const float width = sideWidth / blockPixels;
    const float depth = sideDepth / blockPixels;

    // Half the world dimensions size(centers center of vertices of cube at 0,0)
    const float halfHeight = height / 2.0f;
    const float halfWidth = width / 2.0f;
    const float halfDepth = depth / 2.0f;
    

    switch (dir) {
        case FaceDirection::TOP:
            vert1 = {-halfWidth, halfHeight, -halfDepth};
            vert2 = {halfWidth, halfHeight, -halfDepth};
            vert3 = {halfWidth, halfHeight, halfDepth};
            vert4 = {-halfWidth, halfHeight, halfDepth};
            break;

        case FaceDirection::BOTTOM:
            vert1 = {-halfWidth, -halfHeight, -halfDepth};
            vert2 = {halfWidth, -halfHeight, -halfDepth};
            vert3 = {halfWidth, -halfHeight, halfDepth};
            vert4 = {-halfWidth, -halfHeight, halfDepth};
            break;

        case FaceDirection::NORTH:
            vert1 = {-halfWidth, -halfHeight, -halfDepth};
            vert2 = {halfWidth, -halfHeight, -halfDepth};
            vert3 = {halfWidth, halfHeight, -halfDepth};
            vert4 = {-halfWidth, halfHeight, -halfDepth};
            break;

        case FaceDirection::SOUTH:
            vert1 = {halfWidth, -halfHeight, halfDepth};
            vert2 = {-halfWidth, -halfHeight, halfDepth};
            vert3 = {-halfWidth, halfHeight, halfDepth};
            vert4 = {halfWidth, halfHeight, halfDepth};
            break;
        
        case FaceDirection::EAST:
            vert1 = {halfWidth, -halfHeight, halfDepth};
            vert2 = {halfWidth, -halfHeight, -halfDepth};
            vert3 = {halfWidth, halfHeight, -halfDepth};
            vert4 = {halfWidth, halfHeight, halfDepth};
            break;

        case FaceDirection::WEST:
            vert1 = {-halfWidth, -halfHeight, -halfDepth};
            vert2 = {-halfWidth, -halfHeight, halfDepth};
            vert3 = {-halfWidth, halfHeight, halfDepth};
            vert4 = {-halfWidth, halfHeight, -halfDepth};
            break;
    }

    return {vert1, vert2, vert3, vert4};
}

std::vector<glm::vec2> Cow::getUVCoords(float topLeftX, float topLeftY, float faceHeight, float faceWidth, float texWidth, float texHeight)
{
    // BOTTOM LEFT -> BOTTOM RIGHT -> TOP RIGHT -> BOTTOM LEFT
    glm::vec2 uv1 = {topLeftX / texWidth, (topLeftY + faceHeight) / texHeight};               // BOTTOM LEFT
    glm::vec2 uv2 = {(topLeftX + faceWidth) / texWidth, (topLeftY + faceHeight) / texHeight}; // BOTTOM RIGHT
    glm::vec2 uv3 = {(topLeftX + faceWidth) / texWidth, topLeftY / texHeight};                // TOP RIGHT
    glm::vec2 uv4 = {topLeftX / texWidth, topLeftY / texHeight};                              // TOP LEFT

    return {uv1, uv2, uv3, uv4};
}



void Cow::createHead() {
    const float texWidth = 64.0f;
    const float texHeight = 64.0f;

    const float pixelHeight = 8.0f;
    const float pixelWidth = 8.0f;
    const float pixelDepth = 6.0f;

    float topLeftX, topLeftY;
    float faceHeight, faceWidth;
    FaceDirection direction;


    // TOP FACE
    direction = FaceDirection::TOP;

    topLeftX = 6.0f;
    topLeftY = 0.0f;

    faceHeight = 6.0f;
    faceWidth = 8.0f;

    std::vector<glm::vec3> topVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> topUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert1 = {topVertices[0],  topUVs[0]};
    mobVertex vert2 = {topVertices[1],  topUVs[1]};
    mobVertex vert3 = {topVertices[2],  topUVs[2]};
    mobVertex vert4 = {topVertices[3],  topUVs[3]};





    // Bottom face
    direction = FaceDirection::BOTTOM;
    
    topLeftX = 14.0f;
    topLeftY = 0.0f;

    faceHeight = 6.0f;
    faceWidth = 8.0f;

    std::vector<glm::vec3> bottomVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> bottomUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert5 = {bottomVertices[0],  bottomUVs[0]};
    mobVertex vert6 = {bottomVertices[1],  bottomUVs[1]};
    mobVertex vert7 = {bottomVertices[2],  bottomUVs[2]};
    mobVertex vert8 = {bottomVertices[3],  bottomUVs[3]};





    // North face (face)
    direction = FaceDirection::NORTH;
    topLeftX = 6.0f;
    topLeftY = 6.0f;

    faceHeight = 8.0f;
    faceWidth = 8.0f;


    std::vector<glm::vec3> northVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> northUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert9 = {northVertices[0],  northUVs[0]};
    mobVertex vert10 = {northVertices[1],  northUVs[1]};
    mobVertex vert11 = {northVertices[2],  northUVs[2]};
    mobVertex vert12 = {northVertices[3],  northUVs[3]};




    // South face 
    direction = FaceDirection::SOUTH;

    topLeftX = 16.0f;
    topLeftY = 8.0f;

    faceHeight = 8.0f;
    faceWidth = 8.0f;

    std::vector<glm::vec3> southVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> southUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert13 = {southVertices[0],  southUVs[0]};
    mobVertex vert14 = {southVertices[1],  southUVs[1]};
    mobVertex vert15 = {southVertices[2],  southUVs[2]};
    mobVertex vert16 = {southVertices[3],  southUVs[3]};



    // East face
    direction = FaceDirection::EAST;

    topLeftX = 0.0f;
    topLeftY = 6.0f;

    faceHeight = 8.0f;
    faceWidth = 6.0f;

    std::vector<glm::vec3> eastVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> eastUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert17 = {eastVertices[0],  eastUVs[0]};
    mobVertex vert18 = {eastVertices[1],  eastUVs[1]};
    mobVertex vert19 = {eastVertices[2],  eastUVs[2]};
    mobVertex vert20 = {eastVertices[3],  eastUVs[3]};



    // West face
    direction = FaceDirection::WEST;

    topLeftX = 14.0f;
    topLeftY = 6.0f;

    faceHeight = 8.0f;
    faceWidth = 6.0f;

    std::vector<glm::vec3> westVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> westhUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert21 = {westVertices[0],  westhUVs[0]};
    mobVertex vert22 = {westVertices[1],  westhUVs[1]};
    mobVertex vert23 = {westVertices[2],  westhUVs[2]};
    mobVertex vert24 = {westVertices[3],  westhUVs[3]};

    std::vector<mobVertex> vertices;
    std::vector<GLuint> indices;

    unsigned int baseIndex = vertices.size();

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
        baseIndex + 0, baseIndex + 1, baseIndex + 2,
        baseIndex + 2, baseIndex + 3, baseIndex + 0,
        
        // Bottom face (4-7)
        baseIndex + 4, baseIndex + 5, baseIndex + 6,
        baseIndex + 6, baseIndex + 7, baseIndex + 4,
        
        // North face (8-11)
        baseIndex + 8, baseIndex + 9, baseIndex + 10,
        baseIndex + 10, baseIndex + 11, baseIndex + 8,
        
        // South face (12-15)
        baseIndex + 12, baseIndex + 13, baseIndex + 14,
        baseIndex + 14, baseIndex + 15, baseIndex + 12,
        
        // East face (16-19)
        baseIndex + 16, baseIndex + 17, baseIndex + 18,
        baseIndex + 18, baseIndex + 19, baseIndex + 16,
        
        // West face (20-23)
        baseIndex + 20, baseIndex + 21, baseIndex + 22,
        baseIndex + 22, baseIndex + 23, baseIndex + 20
    });
    headIndicesSize = indices.size();

    headVAO.Bind();
    headVBO.Bind();
    headEBO.Bind();

    headVBO.Upload(vertices.data(), vertices.size() * sizeof(mobVertex));
    headEBO.Upload(indices.data(), indices.size() * sizeof(GLuint));

    headVAO.LinkAttrib(headVBO, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0);
    headVAO.LinkAttrib(headVBO, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float)));
}

void Cow::createBody() {
    const float texWidth = 64.0f;
    const float texHeight = 64.0f;

    const float pixelHeight = 10.0f;
    const float pixelWidth = 12.0f;
    const float pixelDepth = 18.0f;

    float topLeftX, topLeftY;
    float faceHeight, faceWidth;
    FaceDirection direction;


    // TOP FACE
    direction = FaceDirection::TOP;

    topLeftX = 34.0f;
    topLeftY = 14.0f;

    faceHeight = 18.0f;
    faceWidth = 12.0f;

    std::vector<glm::vec3> topVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> topUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert1 = {topVertices[0],  topUVs[0]};
    mobVertex vert2 = {topVertices[1],  topUVs[1]};
    mobVertex vert3 = {topVertices[2],  topUVs[2]};
    mobVertex vert4 = {topVertices[3],  topUVs[3]};


    // Bottom face
    direction = FaceDirection::BOTTOM;
    
    topLeftX = 46.0f;
    topLeftY = 14.0f;

    faceHeight = 18.0f;
    faceWidth = 12.0f;

    std::vector<glm::vec3> bottomVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> bottomUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert5 = {bottomVertices[0],  bottomUVs[0]};
    mobVertex vert6 = {bottomVertices[1],  bottomUVs[1]};
    mobVertex vert7 = {bottomVertices[2],  bottomUVs[2]};
    mobVertex vert8 = {bottomVertices[3],  bottomUVs[3]};





    // North face (face)
    direction = FaceDirection::NORTH;
    topLeftX = 28.0f;
    topLeftY = 4.0f;

    faceHeight = 10.0f;
    faceWidth = 12.0f;


    std::vector<glm::vec3> northVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> northUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert9 = {northVertices[0],  northUVs[0]};
    mobVertex vert10 = {northVertices[1],  northUVs[1]};
    mobVertex vert11 = {northVertices[2],  northUVs[2]};
    mobVertex vert12 = {northVertices[3],  northUVs[3]};




    // South face 
    direction = FaceDirection::SOUTH;

    topLeftX = 40.0f;
    topLeftY = 4.0f;

    faceHeight = 10.0f;
    faceWidth = 12.0f;

    std::vector<glm::vec3> southVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> southUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert13 = {southVertices[0],  southUVs[0]};
    mobVertex vert14 = {southVertices[1],  southUVs[1]};
    mobVertex vert15 = {southVertices[2],  southUVs[2]};
    mobVertex vert16 = {southVertices[3],  southUVs[3]};



    // East face
    direction = FaceDirection::EAST;

    topLeftX = 16.0f;
    topLeftY = 26.0f;

    faceHeight = 10.0f;
    faceWidth = 18.0f;

    std::vector<glm::vec3> eastVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> eastUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert17 = {eastVertices[0],  eastUVs[0]};
    mobVertex vert18 = {eastVertices[1],  eastUVs[1]};
    mobVertex vert19 = {eastVertices[2],  eastUVs[2]};
    mobVertex vert20 = {eastVertices[3],  eastUVs[3]};



    // West face
    direction = FaceDirection::WEST;

    topLeftX = 16.0f;
    topLeftY = 48.0f;

    faceHeight = 10.0f;
    faceWidth = 18.0f;

    std::vector<glm::vec3> westVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> westhUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert21 = {westVertices[0],  westhUVs[0]};
    mobVertex vert22 = {westVertices[1],  westhUVs[1]};
    mobVertex vert23 = {westVertices[2],  westhUVs[2]};
    mobVertex vert24 = {westVertices[3],  westhUVs[3]};

    std::vector<mobVertex> vertices;
    std::vector<GLuint> indices;










    unsigned int baseIndex = vertices.size();

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
        baseIndex + 0, baseIndex + 1, baseIndex + 2,
        baseIndex + 2, baseIndex + 3, baseIndex + 0,
        
        // Bottom face (4-7)
        baseIndex + 4, baseIndex + 5, baseIndex + 6,
        baseIndex + 6, baseIndex + 7, baseIndex + 4,
        
        // North face (8-11)
        baseIndex + 8, baseIndex + 9, baseIndex + 10,
        baseIndex + 10, baseIndex + 11, baseIndex + 8,
        
        // South face (12-15)
        baseIndex + 12, baseIndex + 13, baseIndex + 14,
        baseIndex + 14, baseIndex + 15, baseIndex + 12,
        
        // East face (16-19)
        baseIndex + 16, baseIndex + 17, baseIndex + 18,
        baseIndex + 18, baseIndex + 19, baseIndex + 16,
        
        // West face (20-23)
        baseIndex + 20, baseIndex + 21, baseIndex + 22,
        baseIndex + 22, baseIndex + 23, baseIndex + 20
    });
    bodyIndicesSize = indices.size();

    bodyVAO.Bind();
    bodyVBO.Bind();
    bodyEBO.Bind();

    bodyVBO.Upload(vertices.data(), vertices.size() * sizeof(mobVertex));
    bodyEBO.Upload(indices.data(), indices.size() * sizeof(GLuint));

    bodyVAO.LinkAttrib(bodyVBO, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0);
    bodyVAO.LinkAttrib(bodyVBO, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float)));
}

void Cow::createLeg() {
    const float texWidth = 64.0f;
    const float texHeight = 64.0f;

    const float pixelHeight = 12.0f;
    const float pixelWidth = 4.0f;
    const float pixelDepth = 4.0f;

    float topLeftX, topLeftY;
    float faceHeight, faceWidth;

    FaceDirection direction;


    // =========================== TOP FACE ==============================
    direction = FaceDirection::TOP;

    topLeftX = 4.0f;
    topLeftY = 16.0f;

    faceHeight = 4.0f;
    faceWidth = 4.0f;

    std::vector<glm::vec3> topVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> topUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert1 = {topVertices[0],  topUVs[0]};
    mobVertex vert2 = {topVertices[1],  topUVs[1]};
    mobVertex vert3 = {topVertices[2],  topUVs[2]};
    mobVertex vert4 = {topVertices[3],  topUVs[3]};


    // Bottom face
    direction = FaceDirection::BOTTOM;
    
    topLeftX = 8.0f;
    topLeftY = 16.0f;

    faceHeight = 4.0f;
    faceWidth = 4.0f;

    std::vector<glm::vec3> bottomVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> bottomUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert5 = {bottomVertices[0],  bottomUVs[0]};
    mobVertex vert6 = {bottomVertices[1],  bottomUVs[1]};
    mobVertex vert7 = {bottomVertices[2],  bottomUVs[2]};
    mobVertex vert8 = {bottomVertices[3],  bottomUVs[3]};





    // North face (face)
    direction = FaceDirection::NORTH;

    topLeftX = 0.0f;
    topLeftY = 20.0f;

    faceHeight = 12.0f;
    faceWidth = 4.0f;

    std::vector<glm::vec3> northVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> northUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert9 = {northVertices[0],  northUVs[0]};
    mobVertex vert10 = {northVertices[1],  northUVs[1]};
    mobVertex vert11 = {northVertices[2],  northUVs[2]};
    mobVertex vert12 = {northVertices[3],  northUVs[3]};




    // South face 
    direction = FaceDirection::SOUTH;

    topLeftX = 4.0f;
    topLeftY = 20.0f;

    faceHeight = 12.0f;
    faceWidth = 4.0f;

    std::vector<glm::vec3> southVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> southUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert13 = {southVertices[0],  southUVs[0]};
    mobVertex vert14 = {southVertices[1],  southUVs[1]};
    mobVertex vert15 = {southVertices[2],  southUVs[2]};
    mobVertex vert16 = {southVertices[3],  southUVs[3]};



    // East face
    direction = FaceDirection::EAST;

    topLeftX = 8.0f;
    topLeftY = 20.0f;

    faceHeight = 12.0f;
    faceWidth = 4.0f;

    std::vector<glm::vec3> eastVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> eastUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert17 = {eastVertices[0],  eastUVs[0]};
    mobVertex vert18 = {eastVertices[1],  eastUVs[1]};
    mobVertex vert19 = {eastVertices[2],  eastUVs[2]};
    mobVertex vert20 = {eastVertices[3],  eastUVs[3]};



    // West face
    direction = FaceDirection::WEST;

    topLeftX = 12.0f;
    topLeftY = 20.0f;

    faceHeight = 12.0f;
    faceWidth = 4.0f;

    std::vector<glm::vec3> westVertices = getVertices(pixelHeight, pixelWidth, pixelDepth, direction);
    std::vector<glm::vec2> westhUVs = getUVCoords(topLeftX, topLeftY, faceHeight, faceWidth, texWidth, texHeight);

    mobVertex vert21 = {westVertices[0],  westhUVs[0]};
    mobVertex vert22 = {westVertices[1],  westhUVs[1]};
    mobVertex vert23 = {westVertices[2],  westhUVs[2]};
    mobVertex vert24 = {westVertices[3],  westhUVs[3]};

    std::vector<mobVertex> vertices;
    std::vector<GLuint> indices;







    unsigned int baseIndex = vertices.size();

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
        baseIndex + 0, baseIndex + 1, baseIndex + 2,
        baseIndex + 2, baseIndex + 3, baseIndex + 0,
        
        // Bottom face (4-7)
        baseIndex + 4, baseIndex + 5, baseIndex + 6,
        baseIndex + 6, baseIndex + 7, baseIndex + 4,
        
        // North face (8-11)
        baseIndex + 8, baseIndex + 9, baseIndex + 10,
        baseIndex + 10, baseIndex + 11, baseIndex + 8,
        
        // South face (12-15)
        baseIndex + 12, baseIndex + 13, baseIndex + 14,
        baseIndex + 14, baseIndex + 15, baseIndex + 12,
        
        // East face (16-19)
        baseIndex + 16, baseIndex + 17, baseIndex + 18,
        baseIndex + 18, baseIndex + 19, baseIndex + 16,
        
        // West face (20-23)
        baseIndex + 20, baseIndex + 21, baseIndex + 22,
        baseIndex + 22, baseIndex + 23, baseIndex + 20
    });
    legIndicesSize = indices.size();

    legVAO.Bind();
    legVBO.Bind();
    legEBO.Bind();

    legVBO.Upload(vertices.data(), vertices.size() * sizeof(mobVertex));
    legEBO.Upload(indices.data(), indices.size() * sizeof(GLuint));

    legVAO.LinkAttrib(legVBO, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0);
    legVAO.LinkAttrib(legVBO, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float)));
}
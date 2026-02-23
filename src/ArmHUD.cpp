#include "ArmHUD.h"
#include <cmath>
#include <glm/gtx/string_cast.hpp>

namespace fs = std::filesystem;

ArmHUD::ArmHUD(Shader& s, Camera& c)
    : shader(s),
    armTexture("ResourceFiles/Texture/mobs/steve_atlas.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
    itemAtlas("ResourceFiles/Texture/game_atlas.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
    pos({0.0f, 68.0f, 50.0f}),
    camera(c),
    heldType(c.getPlayerInventory().getHeldItem())
{
    armTexture.texUnit(shader, "tex0", 0);
    itemAtlas.texUnit(shader, "tex0", 0);

    initializeVAO();

    swapHeldItem(c.getPlayerInventory().getHeldItem());

    c.getPlayerInventory().onChange = [this](ItemType held) {
        swapHeldItem(held);
    };
}

void ArmHUD::swapHeldItem(ItemType held) {
    heldType = held;
    if (heldType == ItemType::NONE) {
        createArm();
    }
    else if (heldType == ItemType::ITEM) {
        createHeldItem();
    }
    else if (heldType == ItemType::BLOCK) {
        createHeldBlock();
    }
}

void ArmHUD::renderHand(Camera& camera, float elapsed, const float DT) {
    if (heldType == ItemType::NONE) {
        renderArm(camera, elapsed, DT);
    }
    else if (heldType == ItemType::ITEM) {
        renderItem(camera, elapsed, DT);
    }
    else if (heldType == ItemType::BLOCK) {
        renderBlock(camera, elapsed, DT);
    }
}

void ArmHUD::initializeVAO()
{
    armVAO.Bind();
    armVBO.Bind();
    armEBO.Bind();

    armVAO.LinkAttrib(armVBO, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0);
    armVAO.LinkAttrib(armVBO, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float)));

    armVAO.Unbind();
    armVBO.Unbind();
    armEBO.Unbind();
}

bool ArmHUD::swingArm(glm::vec3& elbowOffset, glm::vec3& wristOffset, float armLength, const float DT) 
{
    glm::vec3 rotateElbowTo = {0.0f, -0.3f, 0.5f};
    glm::vec3 rotateWristTo = glm::normalize(glm::vec3{0.4f, 0.4f, 1.0f}) * armLength;

    static float theta = 0.0f;
    const float pi = glm::pi<float>();

    float thetaRad = glm::radians(theta);

    glm::mat3 rotateElbow = glm::rotate(glm::mat4(1.0f), thetaRad, glm::vec3(0, 0, 1));
    glm::mat3 rotateWrist = glm::rotate(glm::mat4(1.0f), (thetaRad >= 160.0f) ? 0 : thetaRad, glm::vec3(0, 0, 1));

    elbowOffset = rotateElbow * rotateElbowTo;
    wristOffset = rotateWrist * rotateWristTo;

    theta += 7 * 360.0f * DT;
    if (theta >= 360.0f) {
        theta = 0; 
        return false;
    }

    return true;
}

void ArmHUD::renderArm(Camera& camera, float elapsed, const float DT)
{   
    //texture.Bind();
    shader.Activate();
    armTexture.Bind();

    // Camera matrices
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 camMatrix = projection * view;

    // Camera vectors
    glm::vec3 orientation = glm::normalize(camera.getOrientation());
    glm::vec3 position = camera.getPosition();

    // Orthonormal basis for arm
    glm::vec3 right = glm::normalize(glm::cross(orientation, glm::vec3(0,1,0)));
    glm::vec3 up = glm::normalize(glm::cross(right, orientation));
    glm::vec3 down = -up;

    // ============= Shoulder location =================
    glm::vec3 shoulderOffset = 0.5f * right + 0.9f * down - 0.3f * orientation;
    glm::vec3 shoulderPos = position + shoulderOffset;


    // ============= Elbow location =================

    static glm::vec3 elbowOffset = {0.0f, -0.4f, 0.5f};
    glm::vec3 moveTo = glm::vec3{0.0f, -0.4f, 0.5f};

    static bool inSwingAnimation = false;

    if (camera.isArmSwinging() && !inSwingAnimation) {inSwingAnimation = true;} 

    if (!inSwingAnimation && camera.isWalking() && camera.isOnGround) {
        moveTo = {0.0f, -0.1f, 0.5f};

        static float rotate = 0.0f;
        rotate += (camera.isSprinting()) ? 0.048 : 0.038;
        float s = (glm::pi<float>() / 2.0f) * sin(rotate);

        glm::mat3 rotateAxis = glm::rotate(glm::mat4(1.0f), s, glm::vec3(0, 0, 1));
        moveTo = rotateAxis * moveTo;

        moveTo.y -= 0.4f;
    }

    if (elapsed >= DT && !inSwingAnimation) {
        elbowOffset += 0.1f * (moveTo - elbowOffset);
    }

    glm::mat3 changeOrientation = glm::mat4(1.0f);
    changeOrientation[0] = right;
    changeOrientation[1] = up;
    changeOrientation[2] = orientation;

    glm::vec3 newArmPos = changeOrientation * elbowOffset;
    glm::vec3 elbowPos = shoulderPos + newArmPos;


    // ============== Wrist Location ===================

    float armLength = 1.5f;

    // Compute final wrist positionew
    glm::vec3 startPos = glm::vec3(0, 0, 0);
    glm::vec3 dir = glm::normalize(glm::vec3(0.7f, 0.8, 1));
    static glm::vec3 endPos = (startPos + dir) * armLength;

    if (inSwingAnimation && elapsed >= DT) {
        inSwingAnimation = swingArm(elbowOffset, endPos, armLength, DT);
        if (!inSwingAnimation) {
            endPos = (startPos + dir) * armLength;
        }
    }

    glm::mat3 orientationMatrix(1.0f);
    orientationMatrix[0] = right;      // local x-axis
    orientationMatrix[1] = up;         // local y-axis
    orientationMatrix[2] = orientation;    // local z-axis


    glm::vec3 wristOffset = orientationMatrix * endPos;
    glm::vec3 wristPos = elbowPos + wristOffset;


    // ============= Mesh origin =================

    glm::vec3 meshOrigin = wristPos - 0.5f * wristOffset;

    // ===============================================================================
    
    // Draw mesh
    armVAO.Bind();

    glm::vec3 forward = glm::normalize(wristOffset);
    glm::vec3 newRight = glm::normalize(glm::cross(up, forward));
    glm::vec3 newUp = glm::cross(forward, newRight);

    glm::mat4 rotation(1.0f);

    rotation[0] = glm::vec4(newRight, 0.0f);
    rotation[1] = glm::vec4(newUp, 0.0f);
    rotation[2] = glm::vec4(forward, 0.0f);

    rotation = glm::rotate(rotation, (float)glm::radians(-90.0f), glm::vec3(1, 0, 0));    
    rotation = glm::rotate(rotation, (float)glm::radians(54.0f), glm::vec3(0, 1, 0));

    glm::mat4 model = glm::translate(glm::mat4(1.0f), meshOrigin);
    model = glm::scale(model, glm::vec3(1.0f)); 
    model = model * rotation;   

    shader.setMat4("camMatrix", camMatrix * model);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

float angleY = 0.0f;
float angleX = 0.0f;

void ArmHUD::renderItem(Camera& camera, float elapsed, const float DT)
{
    shader.Activate();
    itemAtlas.Bind();

    // Camera matrices
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 camMatrix = projection * view;

    // Camera vectors
    glm::vec3 orientation = glm::normalize(camera.getOrientation());
    glm::vec3 position = camera.getPosition();


    // Orthonormal basis for arm
    glm::vec3 right = glm::normalize(glm::cross(orientation, glm::vec3(0,1,0)));
    glm::vec3 up = glm::normalize(glm::cross(right, orientation));
    glm::vec3 down = -up;

    // ============= Shoulder location =================

    glm::vec3 shoulderOffset = 0.5f * right + 0.9f * down - 0.3f * orientation;
    glm::vec3 shoulderPos = position + shoulderOffset;

    // ============= Elbow location =================

    static glm::vec3 elbowOffset = {0.0f, -0.4f, 0.5f};
    glm::vec3 moveTo = glm::vec3{0.0f, -0.4f, 0.5f};

    static bool inSwingAnimation = false;

    if (camera.isArmSwinging() && !inSwingAnimation) {inSwingAnimation = true;} 

    if (!inSwingAnimation && camera.isWalking() && camera.isOnGround) {
        moveTo = {0.0f, -0.1f, 0.5f};

        static float rotate = 0.0f;
        rotate += (camera.isSprinting()) ? 0.048 : 0.038;
        float s = (glm::pi<float>() / 2.0f) * sin(rotate);

        glm::mat3 rotateAxis = glm::rotate(glm::mat4(1.0f), s, glm::vec3(0, 0, 1));
        moveTo = rotateAxis * moveTo;

        moveTo.y -= 0.4f;
    }

    if (elapsed >= DT && !inSwingAnimation) {
        elbowOffset += 0.1f * (moveTo - elbowOffset);
    }

    glm::mat3 changeOrientation = glm::mat4(1.0f);
    changeOrientation[0] = right;
    changeOrientation[1] = up;
    changeOrientation[2] = orientation;

    glm::vec3 newArmPos = changeOrientation * elbowOffset;
    glm::vec3 elbowPos = shoulderPos + newArmPos;

    // ============== Wrist Location ===================

    float armLength = 1.5f;

    // Compute final wrist position
    glm::vec3 startPos = glm::vec3(0, 0, 0);
    glm::vec3 dir = glm::normalize(glm::vec3(0.7f, 0.8, 1));
    static glm::vec3 endPos = (startPos + dir) * armLength;

    if (inSwingAnimation && elapsed >= DT) {
        inSwingAnimation = swingArm(elbowOffset, endPos, armLength, DT);
        if (!inSwingAnimation) {
            endPos = (startPos + dir) * armLength;
        }
    }

    glm::mat3 orientationMatrix(1.0f);
    orientationMatrix[0] = right;      // local x-axis
    orientationMatrix[1] = up;         // local y-axis
    orientationMatrix[2] = orientation;    // local z-axis


    glm::vec3 wristOffset = orientationMatrix * endPos;
    glm::vec3 wristPos = elbowPos + wristOffset;


    // ============= Mesh origin =================

    glm::vec3 meshOrigin = wristPos - 0.5f * wristOffset;

    // ===============================================================================
    
    // Draw mesh
    armVAO.Bind();

    glm::vec3 forward = glm::normalize(wristOffset);
    glm::vec3 newRight = glm::normalize(glm::cross(up, forward));
    glm::vec3 newUp = glm::cross(forward, newRight);

    glm::mat4 orientMesh = glm::rotate(glm::mat4(1.0f), (float)glm::radians(90.0f), glm::vec3(0, 0, 1));
    orientMesh = glm::rotate(orientMesh, (float)glm::radians(20.0f), glm::vec3(0, 1, 1));

    glm::mat4 rotation(1.0f);
    rotation[0] = glm::vec4(forward, 0.0f);
    rotation[1] = glm::vec4(newUp, 0.0f);
    rotation[2] = glm::vec4(newRight, 0.0f);

    glm::mat4 completeRotation = rotation * orientMesh;

    //rotation = glm::rotate(rotation, (float)glm::radians(angleY), glm::vec3(0, 0, 1));

    glm::mat4 model = glm::translate(glm::mat4(1.0f), meshOrigin + 0.3f * up);
    model = glm::scale(model, glm::vec3(1.0f));
    //model = model * rotation;


    shader.setMat4("camMatrix", camMatrix * model * completeRotation);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void ArmHUD::renderBlock(Camera& camera, float elapsed, const float DT)
{   
    //texture.Bind();
    shader.Activate();
    itemAtlas.Bind();

    // Camera matrices
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 camMatrix = projection * view;

    // Camera vectors
    glm::vec3 orientation = glm::normalize(camera.getOrientation());
    glm::vec3 position = camera.getPosition();


    // Orthonormal basis for arm
    glm::vec3 right = glm::normalize(glm::cross(orientation, glm::vec3(0,1,0)));
    glm::vec3 up = glm::normalize(glm::cross(right, orientation));
    glm::vec3 down = -up;

    // ============= Shoulder location =================

    glm::vec3 shoulderOffset = 0.5f * right + 0.9f * down - 0.3f * orientation;
    glm::vec3 shoulderPos = position + shoulderOffset;


    // ============= Elbow location =================

    static glm::vec3 elbowOffset = {0.0f, -0.4f, 0.5f};
    glm::vec3 moveTo = glm::vec3{0.0f, -0.4f, 0.5f};

    static bool inSwingAnimation = false;

    if (camera.isArmSwinging() && !inSwingAnimation) {inSwingAnimation = true;} 

    if (!inSwingAnimation && camera.isWalking() && camera.isOnGround) {
        moveTo = {0.0f, -0.1f, 0.5f};

        static float rotate = 0.0f;
        rotate += (camera.isSprinting()) ? 0.048 : 0.038;
        float s = (glm::pi<float>() / 2.0f) * sin(rotate);

        glm::mat3 rotateAxis = glm::rotate(glm::mat4(1.0f), s, glm::vec3(0, 0, 1));
        moveTo = rotateAxis * moveTo;

        moveTo.y -= 0.4f;
    }

    if (elapsed >= DT && !inSwingAnimation) {
        elbowOffset += 0.1f * (moveTo - elbowOffset);
    }

    glm::mat3 changeOrientation = glm::mat4(1.0f);
    changeOrientation[0] = right;
    changeOrientation[1] = up;
    changeOrientation[2] = orientation;

    glm::vec3 newArmPos = changeOrientation * elbowOffset;
    glm::vec3 elbowPos = shoulderPos + newArmPos;


    // ============== Wrist Location ===================

    float armLength = 1.5f;

    // Compute final wrist positionew
    glm::vec3 startPos = glm::vec3(0, 0, 0);
    glm::vec3 dir = glm::normalize(glm::vec3(0.7f, 0.8, 1));
    static glm::vec3 endPos = (startPos + dir) * armLength;

    if (inSwingAnimation && elapsed >= DT) {
        inSwingAnimation = swingArm(elbowOffset, endPos, armLength, DT);
        if (!inSwingAnimation) {
            endPos = (startPos + dir) * armLength;
        }
    }

    glm::mat3 orientationMatrix(1.0f);
    orientationMatrix[0] = right;      // local x-axis
    orientationMatrix[1] = up;         // local y-axis
    orientationMatrix[2] = orientation;    // local z-axis


    glm::vec3 wristOffset = orientationMatrix * endPos;
    glm::vec3 wristPos = elbowPos + wristOffset;


    // ============= Mesh origin =================

    glm::vec3 meshOrigin = wristPos - 0.5f * wristOffset;

    // ===============================================================================
    
    // Draw mesh
    armVAO.Bind();

    glm::vec3 forward = glm::normalize(wristOffset);
    glm::vec3 newRight = glm::normalize(glm::cross(up, forward));
    glm::vec3 newUp = glm::normalize(glm::cross(forward, newRight));

    glm::mat4 basisRotation(1.0f);

    basisRotation[0] = glm::vec4(newRight, 0.0f);
    basisRotation[1] = glm::vec4(newUp, 0.0f);
    basisRotation[2] = glm::vec4(forward, 0.0f);

    angleY += 0.1f;
    angleX += 0.1f;

    glm::mat4 localRotation = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 1, 0));
    glm::mat4 secondRotation = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(1, 0, 0));

    glm::mat4 finalRotation = basisRotation * secondRotation * localRotation;


    glm::mat4 model = glm::translate(glm::mat4(1.0f), meshOrigin + 0.2f * newUp);
    model = glm::scale(model, glm::vec3(0.5f)); 

    shader.setMat4("camMatrix", camMatrix * model * finalRotation);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void ArmHUD::createArm() {
    vertices.clear();
    indices.clear();

    float texWidth = 64.0f, texHeight = 64.0f;

    // Arm dimensions: 4 wide, 12 tall, 4 deep (in pixels)
    float armWidth = 0.25f;   // 4 pixels / 16 = 0.25 blocks
    float armHeight = 0.75f;  // 12 pixels / 16 = 0.75 blocks
    float armDepth = 0.25f;

    float x = 0, y = 0, z = 0;

    // FRONT FACE (palm side) - texture at (44, 20), size 4x12
    armVertex vert1 = {{x - armWidth/2, y - armHeight/2, z - armDepth/2}, {44.0f/64.0f, 32.0f/64.0f}};  // Bottom-left
    armVertex vert2 = {{x + armWidth/2, y - armHeight/2, z - armDepth/2}, {48.0f/64.0f, 32.0f/64.0f}};  // Bottom-right
    armVertex vert3 = {{x + armWidth/2, y + armHeight/2, z - armDepth/2}, {48.0f/64.0f, 20.0f/64.0f}};  // Top-right
    armVertex vert4 = {{x - armWidth/2, y + armHeight/2, z - armDepth/2}, {44.0f/64.0f, 20.0f/64.0f}};  // Top-left

    // TOP FACE (shoulder) - texture at (44, 16), size 4x4
    armVertex vert5 = {{x - armWidth/2, y + armHeight/2, z - armDepth/2}, {44.0f/64.0f, 20.0f/64.0f}};
    armVertex vert6 = {{x + armWidth/2, y + armHeight/2, z - armDepth/2}, {48.0f/64.0f, 20.0f/64.0f}};
    armVertex vert7 = {{x + armWidth/2, y + armHeight/2, z + armDepth/2}, {48.0f/64.0f, 16.0f/64.0f}};
    armVertex vert8 = {{x - armWidth/2, y + armHeight/2, z + armDepth/2}, {44.0f/64.0f, 16.0f/64.0f}};

    // BOTTOM FACE (hand) - texture at (48, 16), size 4x4
    armVertex vert9 = {{x - armWidth/2, y - armHeight/2, z - armDepth/2}, {48.0f/64.0f, 20.0f/64.0f}};
    armVertex vert10 = {{x + armWidth/2, y - armHeight/2, z - armDepth/2}, {52.0f/64.0f, 20.0f/64.0f}};
    armVertex vert11 = {{x + armWidth/2, y - armHeight/2, z + armDepth/2}, {52.0f/64.0f, 16.0f/64.0f}};
    armVertex vert12 = {{x - armWidth/2, y - armHeight/2, z + armDepth/2}, {48.0f/64.0f, 16.0f/64.0f}};

    // RIGHT FACE (outer side) - texture at (40, 20), size 4x12
    armVertex vert13 = {{x + armWidth/2, y - armHeight/2, z + armDepth/2}, {40.0f/64.0f, 32.0f/64.0f}};
    armVertex vert14 = {{x + armWidth/2, y - armHeight/2, z - armDepth/2}, {44.0f/64.0f, 32.0f/64.0f}};
    armVertex vert15 = {{x + armWidth/2, y + armHeight/2, z - armDepth/2}, {44.0f/64.0f, 20.0f/64.0f}};
    armVertex vert16 = {{x + armWidth/2, y + armHeight/2, z + armDepth/2}, {40.0f/64.0f, 20.0f/64.0f}};

    // BACK FACE - texture at (52, 20), size 4x12
    armVertex vert17 = {{x + armWidth/2, y - armHeight/2, z + armDepth/2}, {52.0f/64.0f, 32.0f/64.0f}};
    armVertex vert18 = {{x - armWidth/2, y - armHeight/2, z + armDepth/2}, {56.0f/64.0f, 32.0f/64.0f}};
    armVertex vert19 = {{x - armWidth/2, y + armHeight/2, z + armDepth/2}, {56.0f/64.0f, 20.0f/64.0f}};
    armVertex vert20 = {{x + armWidth/2, y + armHeight/2, z + armDepth/2}, {52.0f/64.0f, 20.0f/64.0f}};

    // LEFT FACE (inner side) - texture at (48, 20), size 4x12
    armVertex vert21 = {{x - armWidth/2, y - armHeight/2, z - armDepth/2}, {48.0f/64.0f, 32.0f/64.0f}};
    armVertex vert22 = {{x - armWidth/2, y - armHeight/2, z + armDepth/2}, {52.0f/64.0f, 32.0f/64.0f}};
    armVertex vert23 = {{x - armWidth/2, y + armHeight/2, z + armDepth/2}, {52.0f/64.0f, 20.0f/64.0f}};
    armVertex vert24 = {{x - armWidth/2, y + armHeight/2, z - armDepth/2}, {48.0f/64.0f, 20.0f/64.0f}};

    vertices.insert(vertices.end(), {
        vert1, vert2, vert3, vert4,      // Front
        vert5, vert6, vert7, vert8,      // Top
        vert9, vert10, vert11, vert12,   // Bottom
        vert13, vert14, vert15, vert16,  // Right
        vert17, vert18, vert19, vert20,  // Back
        vert21, vert22, vert23, vert24   // Left
    });

    indices.insert(indices.end(), {
        0,1,2, 2,3,0,    // Front
        4,5,6, 6,7,4,    // Top
        8,9,10, 10,11,8, // Bottom
        12,13,14, 14,15,12, // Right
        16,17,18, 18,19,16, // Back
        20,21,22, 22,23,20  // Left
    });

    armVBO.Upload(vertices.data(), vertices.size() * sizeof(armVertex));
    armEBO.Upload(indices.data(), indices.size() * sizeof(GLuint));
}

void ArmHUD::createHeldItem() {
    vertices.clear();
    indices.clear();

    float itemWidth = 1.0f;
    float itemHeight = 1.0f;
    float itemDepth = 0.0625f;

    PlayerInventory playerInventory = camera.getPlayerInventory();
    Item* heldItem = playerInventory.backpack[playerInventory.heldItemIndex];
    glm::vec2* uvCoords = Item::getUVCoords(heldItem->getID());

    armVertex vert1 = {{-itemWidth / 2, -itemWidth / 2, -itemDepth / 2}, {uvCoords[0]}};
    armVertex vert2 = {{itemWidth / 2, -itemWidth / 2, -itemDepth / 2}, {uvCoords[1]}};
    armVertex vert3 = {{-itemWidth / 2, itemWidth / 2, -itemDepth / 2}, {uvCoords[2]}};
    armVertex vert4 = {{itemWidth / 2, itemWidth / 2, -itemDepth / 2}, {uvCoords[3]}};

    armVertex vert5 = {{-itemWidth / 2, -itemWidth / 2, itemDepth / 2}, {uvCoords[0]}};
    armVertex vert6 = {{itemWidth / 2, -itemWidth / 2, itemDepth / 2}, {uvCoords[1]}};
    armVertex vert7 = {{-itemWidth / 2, itemWidth / 2, itemDepth / 2}, {uvCoords[2]}};
    armVertex vert8 = {{itemWidth / 2, itemWidth / 2, itemDepth / 2}, {uvCoords[3]}};

    vertices.insert(vertices.end(), {
        vert1, vert2, vert3, vert4,
        //vert5, vert6, vert7, vert8
    });

    indices.insert(indices.end(), {
        0, 1, 2,
        1, 3, 2,  // Change this line
        4, 5, 6,
        5, 7, 6   // And this line
    });
    
    armVBO.Upload(vertices.data(), vertices.size() * sizeof(armVertex));
    armEBO.Upload(indices.data(), indices.size() * sizeof(GLuint));
}

void ArmHUD::createHeldBlock() {
    vertices.clear();
    indices.clear();

    float itemWidth = 1.0f;
    float itemDepth = 1.0f;

    PlayerInventory playerInventory = camera.getPlayerInventory();
    Item* heldItem = playerInventory.backpack[playerInventory.heldItemIndex];
    glm::vec2* uvCoords = Item::getUVCoords(heldItem->getID());

    // TOP FACE
    armVertex vert1 = {{-itemWidth / 2, itemWidth / 2, -itemDepth / 2}, {uvCoords[0]}};
    armVertex vert2 = {{itemWidth / 2, itemWidth / 2, -itemDepth / 2}, {uvCoords[1]}};
    armVertex vert3 = {{-itemWidth / 2, itemWidth / 2, itemDepth / 2}, {uvCoords[2]}};
    armVertex vert4 = {{itemWidth / 2, itemWidth / 2, itemDepth / 2}, {uvCoords[3]}};

    // BOTTOM FACE (4-7)
    armVertex vert5 = {{-itemWidth / 2, -itemWidth / 2, -itemDepth / 2}, {uvCoords[0]}};
    armVertex vert6 = {{itemWidth / 2, -itemWidth / 2, -itemDepth / 2}, {uvCoords[1]}};
    armVertex vert7 = {{-itemWidth / 2, -itemWidth / 2, itemDepth / 2}, {uvCoords[2]}};
    armVertex vert8 = {{itemWidth / 2, -itemWidth / 2, itemDepth / 2}, {uvCoords[3]}};

    // NORTH FACE (+Z) (8-11)
    armVertex vert9 = {{-itemWidth / 2, -itemWidth / 2, itemDepth / 2}, {uvCoords[0]}};
    armVertex vert10 = {{itemWidth / 2, -itemWidth / 2, itemDepth / 2}, {uvCoords[1]}};
    armVertex vert11 = {{-itemWidth / 2, itemWidth / 2, itemDepth / 2}, {uvCoords[2]}};
    armVertex vert12 = {{itemWidth / 2, itemWidth / 2, itemDepth / 2}, {uvCoords[3]}};

    // SOUTH FACE (-Z) (12-15)
    armVertex vert13 = {{-itemWidth / 2, -itemWidth / 2, -itemDepth / 2}, {uvCoords[0]}};
    armVertex vert14 = {{itemWidth / 2, -itemWidth / 2, -itemDepth / 2}, {uvCoords[1]}};
    armVertex vert15 = {{-itemWidth / 2, itemWidth / 2, -itemDepth / 2}, {uvCoords[2]}};
    armVertex vert16 = {{itemWidth / 2, itemWidth / 2, -itemDepth / 2}, {uvCoords[3]}};

    // EAST FACE (+X) (16-19)
    armVertex vert17 = {{itemWidth / 2, -itemWidth / 2, -itemDepth / 2}, {uvCoords[0]}};
    armVertex vert18 = {{itemWidth / 2, -itemWidth / 2, itemDepth / 2}, {uvCoords[1]}};
    armVertex vert19 = {{itemWidth / 2, itemWidth / 2, -itemDepth / 2}, {uvCoords[2]}};
    armVertex vert20 = {{itemWidth / 2, itemWidth / 2, itemDepth / 2}, {uvCoords[3]}};

    // WEST FACE (-X) (20-23)
    armVertex vert21 = {{-itemWidth / 2, -itemWidth / 2, -itemDepth / 2}, {uvCoords[0]}};
    armVertex vert22 = {{-itemWidth / 2, -itemWidth / 2, itemDepth / 2}, {uvCoords[1]}};
    armVertex vert23 = {{-itemWidth / 2, itemWidth / 2, -itemDepth / 2}, {uvCoords[2]}};
    armVertex vert24 = {{-itemWidth / 2, itemWidth / 2, itemDepth / 2}, {uvCoords[3]}};

    vertices.insert(vertices.end(), {
        vert1, vert2, vert3, vert4,      // Top (0-3)
        vert5, vert6, vert7, vert8,      // Bottom (4-7)
        vert9, vert10, vert11, vert12,   // North (8-11)
        vert13, vert14, vert15, vert16,  // South (12-15)
        vert17, vert18, vert19, vert20,  // East (16-19)
        vert21, vert22, vert23, vert24   // West (20-23)
    });

    indices.insert(indices.end(), {
        // Top face
        0, 1, 2,
        1, 3, 2,
        // Bottom face
        4, 5, 6,
        5, 7, 6,
        // North face
        8, 9, 10,
        9, 11, 10,
        // South face
        12, 13, 14,
        13, 15, 14,
        // East face
        16, 17, 18,
        17, 19, 18,
        // West face
        20, 21, 22,
        21, 23, 22
    });
    
    armVBO.Upload(vertices.data(), vertices.size() * sizeof(armVertex));
    armEBO.Upload(indices.data(), indices.size() * sizeof(GLuint));
}
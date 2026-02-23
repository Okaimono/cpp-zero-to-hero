#ifndef ARMHUD_H
#define ARMHUD_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"  // Need Camera for renderArm
#include <filesystem>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"  // For transforms
#include "glm/gtc/type_ptr.hpp"          // For value_ptr

struct armVertex {
    glm::vec3 position;
    glm::vec2 texCoord;
};

class ArmHUD {
    public:
        VAO armVAO;
        VBO armVBO;
        EBO armEBO;

        Shader& shader;
        Camera& camera;

        Texture armTexture;
        Texture itemAtlas;

        glm::vec3 pos;

        float bobPhase = 0.0f;
        float swingPhase = 0.0f;

        bool isSwinging = false;

        ArmHUD(Shader& s, Camera& c);

        void renderItem(Camera& camera, float elapsed, const float DT);
        void renderArm(Camera& camera, float elapsed, const float DT);
        void renderHand(Camera& camera, float elapsed, const float DT);
        void renderBlock(Camera& camera, float elapsed, const float DT);
        void matrix();

    private:
        bool swingArm(glm::vec3& elbowOffset, glm::vec3& wristOffset, float armLength, const float DT);

        void createArm();
        void createHeldItem();
        void createHeldBlock();

        void initializeVAO();
        void swapHeldItem(ItemType held);

        void holdItem();

        std::vector<armVertex> vertices;
        std::vector<GLuint> indices;

        ItemType heldType = ItemType::NONE;
};

#endif
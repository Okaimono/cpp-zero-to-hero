// #pragma once

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/rotate_vector.hpp>
// #include <glm/gtx/vector_angle.hpp>

// #include "Camera.h"
// #include "World.h"
// #include "shaderClass.h"

// class PlayerCamera {
// public:
//     PlayerCamera(unsigned int sw, unsigned int sh, glm::vec3 position, World* w, Shader& s, Camera* cam);

//     void Matrix(float FOVdeg, float nearPlane, float farPlane, const char* uniform);

//     Shader* getShaderPointer();

//     glm::mat4 view;
//     glm::mat4 projection;
//     glm::mat4 camMatrix;

//     World* world;
//     Shader& shader;

// };
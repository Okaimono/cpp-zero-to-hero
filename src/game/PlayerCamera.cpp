// #include "PlayerCamera.h"

// PlayerCamera::PlayerCamera(unsigned int sw, unsigned int sh, glm::vec3 position, World* w, Shader& s)
//     : world(w), shader(s)
// {

// }

// void PlayerCamera::Matrix(float FOVdeg, float nearPlane, float farPlane, const char* uniform, Camera* cam)
// {
//     view = glm::mat4(1.0f);
// 	projection = glm::mat4(1.0f);

// 	view = glm::lookAt(Position, Position + Orientation, Up);
// 	projection = glm::perspective(glm::radians(FOVdeg), (float)screenWidth / screenHeight, nearPlane, farPlane);
// 	camMatrix = projection * view;

// 	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(camMatrix));
// }
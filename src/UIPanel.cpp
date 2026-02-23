#include "UIPanel.h"

Shader* UIPanel::uiShader = nullptr;

unsigned UIPanel::screenWidth = 1200;
unsigned UIPanel::screenHeight = 800;




UIPanel::UIPanel(Texture& t)
    : texture(t)
{
    glm::mat4 projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f);
    GLuint projLoc = glGetUniformLocation(uiShader->ID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void UIPanel::render()
{
    vao.Bind();
    texture.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}
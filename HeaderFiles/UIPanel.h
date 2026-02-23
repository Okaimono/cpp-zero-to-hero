#ifndef UIPANEL_H
#define UIPANEL_H


#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Item.h"

#include <string>
#include<memory>
#include<vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<glad/glad.h>


// Goal today:
/*
- Create a UI Panel interface
- Create basic UI's (inventory, etc.)
- Include inventory system, possibily item system too.

*/

/*
PSEUDO CODE/PLANNING

UI panel class
    - Super class, stores VAO, vbo, ebo, basic functions for polymorphism
    - Hold sfunctions to upload vao/vbo/ebo data
    - Holds texture pointers
    

*/


enum class SlotRegion {
    NONE, BACKPACK, CRAFTING, SMELTABLE, FUEL, RESULT, CHEST
};

struct UIVertex {
    glm::vec2 position;
    glm::vec2 texCoord;
};

class UIPanel {
    public:
        static Shader* uiShader;


        std::vector<UIVertex> vertices;
        std::vector<GLuint> indices;

        Texture& texture;
        // Make it so that screenwidth/height are global ref/pointers in future
        static unsigned int screenWidth, screenHeight;

        UIPanel(Texture& t);
        virtual ~UIPanel() {}

        virtual void render();

        void createGUIMesh();

        void initUI();
    protected:
        VAO vao;
        VBO vbo;
        EBO ebo;
};


#endif
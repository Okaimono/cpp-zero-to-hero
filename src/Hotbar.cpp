#include "Hotbar.h"

#include <string>

#include <vector>

Hotbar::Hotbar(PlayerInventory& playerInventory, Texture& hotbarAtlas, Texture& itemAtlas)
    : UIPanel(hotbarAtlas),
    itemDecals(itemAtlas),
    playerInventory(playerInventory)
{
    updateGUI();

    vao.Bind();
    vbo.Bind();
    ebo.Bind();

    vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, 4*sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 4*sizeof(float), (void*)(2*sizeof(float)));

}


void Hotbar::render()
{
    vao.Bind();
    texture.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();

    itemDecals.render();

    renderHotbarText();
}


void Hotbar::updateGUI() {
    vertices.clear();
    indices.clear();

    vao.Bind();
    vbo.Bind();
    ebo.Bind();
    
    addHotbarDecal();
    addSelectedSlotDecal();
    addItemDecals();

    vbo.Upload(vertices.data(), vertices.size() * sizeof(UIVertex));
    ebo.Upload(indices.data(), indices.size() * sizeof(indices));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Hotbar::addHotbarDecal() {
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    float width = 180.0f * scale;  
    float height = 20.0f * scale;
    
    // UV coordinates - try the FULL icon (0,0) to (16,16)
    float atlasSize = 256.0f;

    float iconX = 1.0f;
    float iconY = 1.0f;

    float iconWidth = 180.0f;
    float iconHeight = 20.0f;
    
    float u1 = iconX / atlasSize;              // 2/256 = 0.0078125
    float v1 = iconY / atlasSize;              // 2/256 = 0.0078125
    float u2 = (iconX + iconWidth) / atlasSize; // 182/256 = 0.7109375
    float v2 = (iconY + iconHeight) / atlasSize; // 22/256 = 0.0859375
    
    vertices.insert(vertices.end(), {
        {{centerX - width/2, (hotbarLoc) - height/2},   {u1, v1}},
        {{centerX + width/2, (hotbarLoc) - height/2},   {u2, v1}},
        {{centerX - width/2, (hotbarLoc) + height/2},   {u1, v2}},
        {{centerX + width/2, (hotbarLoc) + height/2},   {u2, v2}}
    });

    indices.insert(indices.end(), {
        0, 1, 2,
        2, 1, 3
    });
}

void Hotbar::addSelectedSlotDecal()
{
    int heldItemIndex = playerInventory.heldItemIndex - 27;
    int heldItemPos = heldItemIndex - 4;

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    // Crosshair size on screen
    float width  = 22.0f * scale;  
    float height = 22.0f * scale;

    float iconWidth =  22.0f;
    float iconHeight = 22.0f;
    
    // UV coordinates - try the FULL icon (0,0) to (16,16)
    float atlasSize = 256.0f;

    float iconX = 1.0f;
    float iconY = 23.0f;

    float u1 = iconX / atlasSize;              
    float v1 = iconY / atlasSize;              
    float u2 = (iconX + iconWidth) / atlasSize;
    float v2 = (iconY + iconHeight) / atlasSize;
 
    // Adds vertices for highlighted box
    vertices.insert(vertices.end(), {
        {{centerX - width/2 + (20 * scale * heldItemPos), (hotbarLoc) - height/2},   {u1, v1}},
        {{centerX + width/2 + (20 * scale * heldItemPos), (hotbarLoc) - height/2},   {u2, v1}},
        {{centerX - width/2 + (20 * scale * heldItemPos), (hotbarLoc) + height/2},   {u1, v2}},
        {{centerX + width/2 + (20 * scale * heldItemPos), (hotbarLoc) + height/2},   {u2, v2}}
    });

    GLuint offset = vertices.size() - 4;
    indices.insert(indices.end(), {
        offset + 0, offset + 1, offset + 2,
        offset + 2, offset + 1, offset + 3
    });
}

void Hotbar::addItemDecals()
{
    itemDecals.vertices.clear();
    itemDecals.indices.clear();

    float width  = 16.0f * scale;  
    float height = 16.0f * scale;

    float startX = (screenWidth / 2.0f) - (9 * 20 * scale) / 2.0f + (2 * scale);
    float startY = hotbarLoc + height/2;

    Item** playerBackpack = playerInventory.backpack;

    for (int i = 27; i < 36; i++) {
        int hotbarPos = i - 27;
        
        if (playerBackpack[i] == nullptr) {
            continue;  // Skip empty slots
        }

        ItemID itemID = playerBackpack[i]->getID();
        glm::vec2* uvs = Item::getUVCoords(itemID);

        float posX = startX + (20 * scale) * hotbarPos;

        itemDecals.vertices.insert(itemDecals.vertices.end(), {
            {{posX, startY - height},   {uvs[0]}},
            {{posX + width, startY - height},   {uvs[1]}},
            {{posX, startY},   {uvs[2]}},
            {{posX + width, startY},   {uvs[3]}}
        });

        GLuint offset = itemDecals.vertices.size() - 4;
        itemDecals.indices.insert(itemDecals.indices.end(), {
            offset + 0, offset + 1, offset + 2,
            offset + 2, offset + 1, offset + 3
        });
    }
    itemDecals.UploadDecalData();
}

void Hotbar::renderHotbarText()
{
    float width  = 16.0f * scale;  
    float height = 16.0f * scale;

    float startX = (screenWidth / 2.0f) - (9 * 20 * scale) / 2.0f + (2 * scale) + 10 * scale;
    float startY = hotbarLoc + height/2 - 10 * scale;

    Item** playerBackpack = playerInventory.backpack;
    


    for (int i = 27; i < 36; i++) {
        if (playerBackpack[i] == nullptr) continue;

        int quantity = playerBackpack[i]->getQuantity();

        if (quantity != 1) {
            int hotbarPos = i - 27;
            int fontLen = 0;
            float posX = startX + (20 * scale) * hotbarPos;
            float posY = startY;

            std::string quantityStr = std::to_string(quantity);

            if (quantityStr.size() == 2) {
                fontLen = 5.333 * scale;
            }

            ImGui::GetForegroundDrawList()->AddText(
                nullptr,               // Default font
                10.667 * scale,                 // Font size (scale up as needed)
                ImVec2(posX - fontLen, posY),    // Screen position
                IM_COL32(255, 255, 255, 255), // Color: white
                quantityStr.c_str()                    // Text to draw
            );
        }
    }
}
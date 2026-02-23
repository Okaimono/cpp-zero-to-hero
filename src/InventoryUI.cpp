#include "InventoryUI.h"

#include "UIPanel.h"

#include <optional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

InventoryUI::InventoryUI(PlayerInventory& playerInventory, Texture& inventoryAtlas, Texture& itemAtlas)
    : UIPanel(inventoryAtlas), 
    playerInventory(playerInventory),
    itemDecals(itemAtlas),
    selectedItemDecal(itemAtlas)
{   
    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            craftingStorage[x][y] = nullptr;
        }
    }
    craftingResult = nullptr;

    updateGUI();

    vao.Bind();
    vbo.Bind();
    ebo.Bind();
    
    vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, 4*sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 4*sizeof(float), (void*)(2*sizeof(float)));
}

void InventoryUI::updateGUI()
{
    setInventoryUI();
    setItemDecals(0.0f, 0.0f);
}


void InventoryUI::render()
{
    vao.Bind();
    texture.Bind();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();

    itemDecals.render();
    renderInventoryText();
    // selectedItemDecal.render();
    // renderSelectedItemText();
}

void InventoryUI::renderInventoryText()
{
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    // decal size 
    float width  = 16.0f * scale;  
    float height = 16.0f * scale;

    float iconWidth =  16.0f;
    float iconHeight = 16.0f;
    
    float atlasSize = 256.0f;


    float inventoryWidth = 175.0f * scale;  
    float inventoryHeight = 164.0f * scale;

    // OpenGL: bottom-left origin, Y points UP
    float inventoryBottomLeftX = centerX - (inventoryWidth/2);
    float inventoryBottomLeftY = centerY - (inventoryHeight/2);

    // Start at bottom-left of first slot
    // 8*3 from left edge, 84*3 UP from bottom
    float inventoryStartX = inventoryBottomLeftX + 8.0f * scale;
    float inventoryStartY = inventoryBottomLeftY + 84.0f * scale;

    float increment = 18.0f * scale;

    Item** backpack = playerInventory.backpack;

    // Puts text on inventory
    for (int i = 0; i < 36; i++) {
        Item* item = backpack[i];
        if (item == nullptr) {continue;}

        int itemQuantity = item->getQuantity();
        if (itemQuantity == 1) {continue;}

        ItemID itemID = item->getID();

        int backpackIndexY = i / 9;
        int backpackIndexX = i - (9 * backpackIndexY);

        // 10 * scale shifts text to proper position from bottom left corner
        float posX = inventoryStartX + (backpackIndexX * increment) + 10 * scale;
        float posY = inventoryStartY + (backpackIndexY * increment) + 6 * scale;
        if (backpackIndexY == 3) {
            posY += 4 * scale;
        }

        addText(posX, posY, itemQuantity);
    }

    float craftingStartX = inventoryBottomLeftX + 98.0f * scale;
    float craftingStartY = inventoryBottomLeftY + 18.0f * scale;

    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            if (craftingStorage[row][col] != nullptr) {
                int itemQuantity = craftingStorage[row][col]->getQuantity();
                if (itemQuantity == 1) {continue;}

                // 10 * scale shifts text to proper position from bottom left corner
                float posX = craftingStartX + (increment * col) + 10 * scale;
                float posY = craftingStartY + (increment * row) + 6 * scale;

                addText(posX, posY, itemQuantity);
            }
        }
    }

    float craftingResultStartX = inventoryBottomLeftX + 154.0f * scale;
    float craftingResultStartY = inventoryBottomLeftY + 28.0f * scale;

    // Crafting result text
    if (craftingResult != nullptr) {
        int itemQuantity = craftingResult->getQuantity();

        if (itemQuantity != 1) {
            float posX = craftingResultStartX + 10 * scale;
            float posY = craftingResultStartY + 6 * scale;

            addText(posX, posY, itemQuantity);
        }
    }
    if (itemSelected != nullptr && *itemSelected) {
        int itemQuantity = selectedItem->getQuantity();
        
        if (itemQuantity != 1) {
            float posX = mouseX + 10 * scale;
            float posY = (screenHeight - mouseY) + 6 * scale;
            
            addText(posX, posY, itemQuantity);
        }
    }
}

void InventoryUI::addText(float posX, float posY, int quantity)
{

    std::string quantityStr = std::to_string(quantity);

    int fontLen;

    if (quantityStr.size() == 1) {
        fontLen = 0;
    }
    if (quantityStr.size() == 2) {
        fontLen = 5.333 * scale;
    }

    ImGui::GetForegroundDrawList()->AddText(
        nullptr,               // Default font
        10.667f * scale,                 // Font size (scale up as needed)
        ImVec2(posX - fontLen, posY),    // Screen position
        IM_COL32(255, 255, 255, 255), // Color: white
        quantityStr.c_str()                    // Text to draw
    );
}

void InventoryUI::playerClick(float glX, float glY, bool* itemSelected, bool leftClick)
{
    updateInventoryPosition();

    float slotTransformX;
    float slotTransformY;

    float slotSpacing = 18.0f * scale;;  // 54 pixels

    float startSlotX;
    float startSlotY;

    int slotCol;
    int slotRow;

    int numCols;

    // If they clicked in backpack
    if (getClickedSection(glX, glY) == SlotRegion::BACKPACK) {
        slotTransformX = 8.0f * scale;
        slotTransformY = 62.0f * scale;

        startSlotX = inventoryBottomLeftX + slotTransformX;
        startSlotY = inventoryBottomLeftY + slotTransformY + (1 * slotSpacing);  // only 1 row up

        slotCol = (glX - startSlotX) / slotSpacing;
        slotRow = (startSlotY - glY) / slotSpacing;  // down = positive row

        Item** backpack = playerInventory.backpack;
        int backpackIndex = slotCol + slotRow * 9;

        numCols = 9;

        if (leftClick) {
            interactLeftClick(backpack, slotCol, slotRow, numCols);
        }
        else if (!leftClick) {
            interactRightClick(backpack, slotCol, slotRow, numCols);
        }
        setItemDecals(0.0f, 0.0f);
    }

    else if (getClickedSection(glX, glY) == SlotRegion::CRAFTING) {
        slotTransformX = 98.0f * scale;
        slotTransformY = 129.0f * scale;

        startSlotX = inventoryBottomLeftX + slotTransformX;
        startSlotY = inventoryBottomLeftY + slotTransformY + (1 * slotSpacing);  // only 1 row up

        slotCol = (glX - startSlotX) / slotSpacing;
        slotRow = (startSlotY - glY) / slotSpacing;  // down = positive row

        numCols = 2;

        if (leftClick) {
            interactLeftClick((Item**)craftingStorage, slotCol, slotRow, numCols);
        }
        else if (!leftClick) {
            interactRightClick((Item**)craftingStorage, slotCol, slotRow, numCols);
        }
        checkCraft();
        setItemDecals(0.0f, 0.0f);
    }

    if (getClickedSection(glX, glY) == SlotRegion::RESULT) {
        slotCol = (glX - startSlotX) / slotSpacing;
        slotRow = (startSlotY - glY) / slotSpacing;  // down = positive row
        interactCraftResult(&craftingResult, slotCol, slotRow);

        checkCraft();
        setItemDecals(0.0f, 0.0f);
    }
}

void InventoryUI::interactLeftClick(Item** slots, int slotCol, int slotRow, int numCols)
{

    int index = slotCol + slotRow * numCols;
    if (*itemSelected == false && slots[index] != nullptr) {
        *itemSelected = true;
        selectedItem = slots[index];
        slots[index] = nullptr;
    }
    // Player clicks on empty slot while having item selected
    else if (*itemSelected == true && slots[index] == nullptr) {
        *itemSelected = false;
        slots[index] = selectedItem;
        selectedItem = nullptr;
    }
    // Player left clicks on a slot with item while having item selected
    else if (*itemSelected == true && slots[index] != nullptr) {
        if (selectedItem->getID() == slots[index]->getID()) {
            bool isItemEmpty = selectedItem->putOnSlot(slots[index]);
            if (isItemEmpty) {
                *itemSelected = false;
                delete selectedItem;
                selectedItem = nullptr;
            }
        }
        else {
            Item* tempItem = selectedItem;
            selectedItem = slots[index];
            slots[index] = tempItem;
        }
    }
}

void InventoryUI::interactRightClick(Item** slots, int slotCol, int slotRow, int numCols)
{
    int index = slotCol + slotRow * numCols;
    if (*itemSelected == false && slots[index] != nullptr) {
        *itemSelected = true;
        if (slots[index]->getQuantity() > 1) {
            int larger = slots[index]->getQuantity() / 2;
            selectedItem = new Item(slots[index]->getID(), larger);
            bool isEmpty = slots[index]->subtractQuantity(larger);
            if (isEmpty) {
                delete slots[index];
                slots[index] = nullptr;
            }
        }
        else if (slots[index]->getQuantity() == 1) {
            selectedItem = slots[index];
            slots[index] = nullptr;
        }
    }
    else if (*itemSelected == true && slots[index] == nullptr) {
        slots[index] = new Item(selectedItem->getID());

        bool isEmpty = selectedItem->subtractQuantity(1);
        if (isEmpty) {
            *itemSelected = false;
            delete selectedItem;
            selectedItem = nullptr;
        }
    }
    else if (*itemSelected == true && slots[index] != nullptr && slots[index]->getID() == selectedItem->getID() && slots[index]->getQuantity() != slots[index]->getMaxQuantity()) {
        slots[index]->addQuantity(1);

        bool isEmpty = selectedItem->subtractQuantity(1);
        if (isEmpty) {
            *itemSelected = false;
            delete selectedItem;
            selectedItem = nullptr;
        }
    }
    else if (*itemSelected == true && slots[index] != nullptr && slots[index]->getID() != selectedItem->getID()) {
        Item* tempItem = selectedItem;
        selectedItem = slots[index];
        slots[index] = tempItem;
    }
}

void InventoryUI::interactCraftResult(Item** slots, int slotCol, int slotRow)
{
    if (craftingResult != nullptr)
    {
        Crafting::completeCraft<2>(craftingStorage, craftingResult, selectedItem, itemSelected);
    }
}

void InventoryUI::updateInventoryPosition() {
    float inventoryWidth = 175.0f * scale;
    float inventoryHeight = 164.0f * scale;
    float centerX = screenWidth / 2;
    float centerY = screenHeight / 2;
    inventoryBottomLeftX = centerX - inventoryWidth / 2;
    inventoryBottomLeftY = centerY - inventoryHeight / 2;
}


SlotRegion InventoryUI::getClickedSection(float glX, float glY) 
{  
    float slotTransformX;
    float slotTransformY;

    const float slotSpacing = 18.0f * scale; ;   // 54 pixels

    // Start at TOP-LEFT slot (row 0, col 0) - shifted down one row
    float startSlotX;
    float startSlotY;  // only 1 row up

    int slotCol;
    int slotRow;  // down = positive row

    float rangeX;
    float rangeY;

    float numCols;
    float numRows;

    bool rowOffset = false;
    
//  == Backpack ===================================================


    slotTransformX = 8.0f * scale;
    slotTransformY = 62.0f * scale;

    startSlotX = inventoryBottomLeftX + slotTransformX;
    startSlotY = inventoryBottomLeftY + slotTransformY + (1 * slotSpacing);  // only 1 row up

    slotCol = (glX - startSlotX) / slotSpacing;
    slotRow = (startSlotY - glY) / slotSpacing;  // down = positive row

    rangeX = fmod(glX - startSlotX, slotSpacing);
    rangeY = fmod(startSlotY - glY, slotSpacing);

    numCols = 9;
    numRows = 4;

    rowOffset = true;

    bool checkIfInventory = checkRegion(slotCol, slotRow, numCols, numRows, rangeX, rangeY, rowOffset);

    if (checkIfInventory) {
        return SlotRegion::BACKPACK;
    }

//  == Crafting Space ==============================================

    slotTransformX = 98.0f * scale;
    slotTransformY = 129.0f * scale;

    startSlotX = inventoryBottomLeftX + slotTransformX;
    startSlotY = inventoryBottomLeftY + slotTransformY + (1 * slotSpacing);  // only 1 row up

    slotCol = (glX - startSlotX) / slotSpacing;
    slotRow = (startSlotY - glY) / slotSpacing;  // down = positive row

    rangeX = fmod(glX - startSlotX, slotSpacing);
    rangeY = fmod(startSlotY - glY, slotSpacing);

    numCols = 2;
    numRows = 2;

    rowOffset = false;

    bool checkIfCrafting = checkRegion(slotCol, slotRow, numCols, numRows, rangeX, rangeY, rowOffset);

    if (checkIfCrafting) {
        return SlotRegion::CRAFTING;
    }

//  == Result Space ================================================

    slotTransformX = 154.0f * scale;
    slotTransformY = 118.0f * scale;

    startSlotX = inventoryBottomLeftX + slotTransformX;
    startSlotY = inventoryBottomLeftY + slotTransformY + (1 * slotSpacing);  // only 1 row up

    slotCol = (glX - startSlotX) / slotSpacing;
    slotRow = (startSlotY - glY) / slotSpacing;  // down = positive row

    rangeX = fmod(glX - startSlotX, slotSpacing);
    rangeY = fmod(startSlotY - glY, slotSpacing);

    numCols = 1;
    numRows = 1;

    rowOffset = false;

    bool checkIfResult = checkRegion(slotCol, slotRow, numCols, numRows, rangeX, rangeY, rowOffset);

    if (checkIfResult) {
        return SlotRegion::RESULT;
    }

//  =================================================

    return SlotRegion::NONE;
}

bool InventoryUI::checkRegion(int slotCol, int slotRow, float numCols, float numRows, float rangeX, float rangeY, bool rowOffset) {
    const float slotLength = 16.0f * scale;
    const float slotHeight = 16.0f * scale;  

    if (slotCol >= 0 && slotCol < numCols && slotRow >= 0 && slotRow < numRows) {
        float offset = 0;
        if (slotCol == 3 && rowOffset) {
            offset = 4 * scale;
        }
        if (rangeX > 0 && rangeX < slotLength && 
            (rangeY > 0 + offset) && (rangeY < slotHeight + offset)) {
            return true;
        }
    }
    return false;
}

void InventoryUI::checkCraft() {
    Item* newCraftingResult = Crafting::canCraft<2>(craftingStorage);

    // If there wasn't a crafting result before, put the new one there.
    if (craftingResult == nullptr && newCraftingResult != nullptr) {
        craftingResult = newCraftingResult;
    }
    // If the new crafting item result is not the same as the old one
    else if (craftingResult != nullptr && newCraftingResult != nullptr && newCraftingResult->getID() != craftingResult->getID()) {
        delete craftingResult;
        craftingResult = newCraftingResult;
    }
    else if (craftingResult != nullptr && newCraftingResult != nullptr && newCraftingResult->getID() == craftingResult->getID()) {
        delete newCraftingResult;
        newCraftingResult = nullptr;
    }
    else if (craftingResult != nullptr && newCraftingResult == nullptr) {
        delete craftingResult;
        craftingResult = nullptr;
    }   
    else if (newCraftingResult != nullptr) {
        delete newCraftingResult;
    }
    setItemDecals(0.0f, 0.0f);
}

void InventoryUI::setInventoryUI()
{
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    // Inventory UI size on screen
    float width = 175.0f * scale;  
    float height = 164.0f * scale;
    
    // UV coordinates - try the FULL icon (0,0) to (16,16)
    float atlasSize = 256.0f;

    // Start coords on atlas
    float iconX = 0.0f;
    float iconY = 0.0f;

    // Length/width in pixels on atlas
    float iconWidth = 175.0f;
    float iconHeight = 164.0f;
    
    float u1 = iconX / atlasSize;              // 2/256 = 0.0078125
    float v1 = iconY / atlasSize;              // 2/256 = 0.0078125
    float u2 = (iconX + iconWidth) / atlasSize; // 182/256 = 0.7109375
    float v2 = (iconY + iconHeight) / atlasSize; // 22/256 = 0.0859375

    vertices.insert(vertices.end(), {
        {{centerX - width/2, centerY - height/2},   {u1, v1}},
        {{centerX + width/2, centerY - height/2},   {u2, v1}},
        {{centerX - width/2, centerY + height/2},   {u1, v2}},
        {{centerX + width/2, centerY + height/2},   {u2, v2}}
    });

    indices.insert(indices.end(), {
        0, 1, 2,
        2, 1, 3
    });

    vbo.Upload(vertices.data(), vertices.size() * sizeof(UIVertex));
    ebo.Upload(indices.data(), indices.size() * sizeof(indices));
}

void InventoryUI::setItemDecals(float glX, float glY)
{
    mouseX = glX;
    mouseY = glY;

    itemDecals.vertices.clear();
    itemDecals.indices.clear();

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    // decal size 
    float width  = 16.0f * scale;  
    float height = 16.0f * scale;

    float iconWidth =  16.0f;
    float iconHeight = 16.0f;
    
    float atlasSize = 256.0f;


    float inventoryWidth = 175.0f * scale;  
    float inventoryHeight = 164.0f * scale;

    // OpenGL: bottom-left origin, Y points UP
    float inventoryBottomLeftX = centerX - (inventoryWidth/2);
    float inventoryBottomLeftY = centerY - (inventoryHeight/2);

    // Start at bottom-left of first slot
    // 8*3 from left edge, 84*3 UP from bottom
    float inventoryStartX = inventoryBottomLeftX + 8.0f * scale;
    float inventoryStartY = inventoryBottomLeftY + 84.0f * scale;

    float increment = 18.0f * scale;

    Item** backpack = playerInventory.backpack;


    // Put backpack item decals on the interface

    for (int i = 0; i < 36; i++) {


        Item* item = backpack[i];
        if (item == nullptr) {continue;}

        ItemID itemID = item->getID();

        float iconX;
        float iconY;

        int backpackIndexY = i / 9;
        int backpackIndexX = i - (9 * backpackIndexY);

        glm::vec2* uvs = Item::getUVCoords(itemID);

        UIVertex vert1 = { { inventoryStartX + (increment * backpackIndexX), inventoryStartY + (increment * backpackIndexY) },   {uvs[0]} };
        UIVertex vert2 = { { inventoryStartX + (increment * backpackIndexX) + width, inventoryStartY + (increment * backpackIndexY) },   {uvs[1]} };
        UIVertex vert3 = { { inventoryStartX + (increment * backpackIndexX), inventoryStartY + (increment * backpackIndexY) + height },   {uvs[2] } };
        UIVertex vert4 = { { inventoryStartX + (increment * backpackIndexX) + width, inventoryStartY + (increment * backpackIndexY) + height },   {uvs[3]} };

        if (backpackIndexY == 3) {
            vert1.position.y += 4 * scale;
            vert2.position.y += 4 * scale;
            vert3.position.y += 4 * scale;
            vert4.position.y += 4 * scale;
        }

        itemDecals.vertices.insert(itemDecals.vertices.end(), {
            // Bottom-left
            vert1,
            vert2,
            vert3,
            vert4
        });

        GLuint offset = itemDecals.vertices.size() - 4;
        itemDecals.indices.insert(itemDecals.indices.end(), {
            offset + 0, offset + 1, offset + 2,
            offset + 2, offset + 1, offset + 3
        });
    }




    // Put decals on crafting interface
    inventoryStartX = inventoryBottomLeftX + 98.0f * scale;
    inventoryStartY = inventoryBottomLeftY + 18.0f * scale;

    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            if (craftingStorage[row][col] != nullptr) {
                ItemID itemID = craftingStorage[row][col]->getID();
                glm::vec2* uvs = Item::getUVCoords(itemID);

                float posX = inventoryStartX + (increment * col);
                float posY = inventoryStartY + (increment * row);

                UIVertex vert1 = { { posX, posY }, {uvs[0]} };
                UIVertex vert2 = { { posX + width, posY }, {uvs[1]} };
                UIVertex vert3 = { { posX, posY + height }, {uvs[2]} };
                UIVertex vert4 = { { posX + width, posY + height }, {uvs[3]} };

                itemDecals.vertices.insert(itemDecals.vertices.end(), {
                    vert1, vert2, vert3, vert4
                });

                GLuint offset = itemDecals.vertices.size() - 4;
                itemDecals.indices.insert(itemDecals.indices.end(), {
                    offset + 0, offset + 1, offset + 2,
                    offset + 2, offset + 1, offset + 3
                });
            }
        }
    }

    inventoryStartX = inventoryBottomLeftX + 154.0f * scale;
    inventoryStartY = inventoryBottomLeftY + 28.0f * scale;

    // Crafting Result
    if (craftingResult != nullptr) {
        float iconX;
        float iconY;

        ItemID itemID = craftingResult->getID();
        glm::vec2* uvs = Item::getUVCoords(itemID);

        UIVertex vert1 = { { inventoryStartX, inventoryStartY },   {uvs[0]} };
        UIVertex vert2 = { { inventoryStartX + width, inventoryStartY},   { uvs[1]} };
        UIVertex vert3 = { { inventoryStartX, inventoryStartY + height },   { uvs[2] } };
        UIVertex vert4 = { { inventoryStartX + width, inventoryStartY + height },   { uvs[3] } };

        itemDecals.vertices.insert(itemDecals.vertices.end(), {
            // Bottom-left
            vert1,
            vert2,
            vert3,
            vert4
        });

        GLuint offset = itemDecals.vertices.size() - 4;
        itemDecals.indices.insert(itemDecals.indices.end(), {
            offset + 0, offset + 1, offset + 2,
            offset + 2, offset + 1, offset + 3
        });

    }


    if (itemSelected != nullptr && *itemSelected) {
        float iconX;
        float iconY;

        ItemID itemID = selectedItem->getID();

        glm::vec2* uvs = Item::getUVCoords(itemID);

        itemDecals.vertices.insert(itemDecals.vertices.end(), {
            { {glX, screenHeight - glY}, {uvs[0]} },                    // bottom-left
            { {glX + width, screenHeight - glY}, {uvs[1]} },            // bottom-right
            { {glX, screenHeight - (glY - height)}, {uvs[2]} },         // top-left
            { {glX + width, screenHeight - (glY - height)}, {uvs[3]} }  // top-right
        });

        GLuint offset = itemDecals.vertices.size() - 4;
        itemDecals.indices.insert(itemDecals.indices.end(), {
            offset + 0, offset + 1, offset + 2,
            offset + 2, offset + 1, offset + 3
        });
    }

    itemDecals.UploadDecalData();

}

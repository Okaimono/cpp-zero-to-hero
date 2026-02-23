// FurnaceUI.cpp
#include "FurnaceUI.h"

#include "UIPanel.h"

#include <optional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

FurnaceUI::FurnaceUI(PlayerInventory& playerInventory, Texture& inventoryAtlas, Texture& itemAtlas)
    : UIPanel(inventoryAtlas), 
    playerInventory(playerInventory),
    itemDecals(itemAtlas)
{   
    selectedItem = nullptr;

    updateGUI();

    vao.Bind();
    vbo.Bind();
    ebo.Bind();
    
    vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, 4*sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 4*sizeof(float), (void*)(2*sizeof(float)));
}

void FurnaceUI::renderInventoryText()
{
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    float width  = 16.0f * scale;  
    float height = 16.0f * scale;

    float iconWidth =  16.0f;
    float iconHeight = 16.0f;
    
    float atlasSize = 256.0f;

    float inventoryWidth = 175.0f * scale;  
    float inventoryHeight = 164.0f * scale;

    float inventoryBottomLeftX = centerX - (inventoryWidth/2);
    float inventoryBottomLeftY = centerY - (inventoryHeight/2);

    float inventoryStartX = inventoryBottomLeftX + 8.0f * scale;
    float inventoryStartY = inventoryBottomLeftY + 84.0f * scale;

    float increment = 18.0f * scale;

    Item** backpack = playerInventory.backpack;

    for (int i = 0; i < 36; i++) {
        Item* item = backpack[i];
        if (item == nullptr) {continue;}

        int itemQuantity = item->getQuantity();
        if (itemQuantity == 1) {continue;}

        ItemID itemID = item->getID();

        int backpackIndexY = i / 9;
        int backpackIndexX = i - (9 * backpackIndexY);

        float posX = inventoryStartX + (backpackIndexX * increment) + 10 * scale;
        float posY = inventoryStartY + (backpackIndexY * increment) + 6 * scale;
        if (backpackIndexY == 3) {
            posY += 4 * scale;
        }

        addText(posX, posY, itemQuantity);
    }

    if (bindedFurnace == nullptr) {return;}

    float furnaceSmeltableStartX = inventoryBottomLeftX + 56.0f * scale;
    float furnaceSmeltableStartY = inventoryBottomLeftY + 17.0f * scale;

    if (bindedFurnace->smeltable != nullptr) {
        int itemQuantity = bindedFurnace->smeltable->getQuantity();

        if (itemQuantity != 1) {
            float posX = furnaceSmeltableStartX + 10 * scale;
            float posY = furnaceSmeltableStartY + 6 * scale;

            addText(posX, posY, itemQuantity);
        }
    }

    float fuelStartX = inventoryBottomLeftX + 56.0f * scale;
    float fuelStartY = inventoryBottomLeftY + 53.0f * scale;

    if (bindedFurnace->fuel != nullptr) {
        int itemQuantity = bindedFurnace->fuel->getQuantity();

        if (itemQuantity != 1) {
            float posX = fuelStartX + 10 * scale;
            float posY = fuelStartY + 6 * scale;
            
            addText(posX, posY, itemQuantity);
        }
    }
    
    float resultStartX = inventoryBottomLeftX + 117.0f * scale;
    float resultStartY = inventoryBottomLeftY + 35.0f * scale;

    if (bindedFurnace->result != nullptr) {
        int itemQuantity = bindedFurnace->result->getQuantity();

        if (itemQuantity != 1) {
            float posX = resultStartX + 10 * scale;
            float posY = resultStartY + 6 * scale;

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

void FurnaceUI::addText(float posX, float posY, int quantity)
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
        nullptr,
        10.667f * scale,
        ImVec2(posX - fontLen, posY),
        IM_COL32(255, 255, 255, 255),
        quantityStr.c_str()
    );
}

bool FurnaceUI::checkIfSmelting()
{
    if (bindedFurnace != nullptr && bindedFurnace->smelting == true)
    {
        return true;
    }
    return false;
}

void FurnaceUI::bindFurnace(Furnace* newFurnace)
{
    bindedFurnace = newFurnace;
}

void FurnaceUI::updateGUI()
{
    setFurnaceUI();
    setItemDecals(0.0f, 0.0f);
}

void FurnaceUI::render()
{
    vao.Bind();
    texture.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();

    itemDecals.render();
    renderInventoryText();
}

void FurnaceUI::playerClick(float glX, float glY, bool* itemSelected, bool leftClick)
{
    updateInventoryPosition();

    float slotTransformX;
    float slotTransformY;

    float slotSpacing = 18.0f * scale;

    float startSlotX;
    float startSlotY;

    int slotCol;
    int slotRow;

    int numCols;

    if (getClickedSection(glX, glY) == SlotRegion::BACKPACK) {
        slotTransformX = 8.0f * scale;
        slotTransformY = 62.0f * scale;

        startSlotX = inventoryBottomLeftX + slotTransformX;
        startSlotY = inventoryBottomLeftY + slotTransformY + (1 * slotSpacing);

        slotCol = (glX - startSlotX) / slotSpacing;
        slotRow = (startSlotY - glY) / slotSpacing;

        Item** backpack = playerInventory.backpack;
        numCols = 9;

        if (leftClick) {
            interactLeftClick(backpack, slotCol, slotRow, numCols);
        }
        else if (!leftClick) {
            interactRightClick(backpack, slotCol, slotRow, numCols);
        }
        setItemDecals(0.0f, 0.0f);
    }

    else if (getClickedSection(glX, glY) == SlotRegion::SMELTABLE) {
        if (bindedFurnace == nullptr) return;
        
        Item** smeltableSlot = &bindedFurnace->smeltable;
        numCols = 1;
        slotCol = 0;
        slotRow = 0;
        
        if (leftClick) {
            if (*itemSelected && checkIfSmeltable(selectedItem->getID())) {
                interactLeftClick(smeltableSlot, slotCol, slotRow, numCols);
            }
            else if (!*itemSelected) {
                interactLeftClick(smeltableSlot, slotCol, slotRow, numCols);
            }
        }
        else if (!leftClick) {
            if (*itemSelected && checkIfSmeltable(selectedItem->getID())) {
                interactRightClick(smeltableSlot, slotCol, slotRow, numCols);
            }
            else if (!*itemSelected) {
                interactRightClick(smeltableSlot, slotCol, slotRow, numCols);
            }
        }
        setItemDecals(glX, glY);
    }

    else if (getClickedSection(glX, glY) == SlotRegion::FUEL) {
        if (bindedFurnace == nullptr) return;
        
        Item** fuelSlot = &bindedFurnace->fuel;
        numCols = 1;
        slotCol = 0;
        slotRow = 0;
        
        if (leftClick) {
            if (*itemSelected && checkIfFuel(selectedItem->getID())) {
                interactLeftClick(fuelSlot, slotCol, slotRow, numCols);
            }
            else if (!*itemSelected) {
                interactLeftClick(fuelSlot, slotCol, slotRow, numCols);
            }
        }
        else if (!leftClick) {
            if (*itemSelected && checkIfFuel(selectedItem->getID())) {
                interactRightClick(fuelSlot, slotCol, slotRow, numCols);
            }
            else if (!*itemSelected) {
                interactRightClick(fuelSlot, slotCol, slotRow, numCols);
            }
        }
        setItemDecals(glX, glY);
    }

    else if (getClickedSection(glX, glY) == SlotRegion::RESULT) {
        if (bindedFurnace == nullptr) return;
        
        Item** resultSlot = &bindedFurnace->result;
        numCols = 1;
        slotCol = 0;
        slotRow = 0;
        
        if (leftClick) {
            interactLeftClick(resultSlot, slotCol, slotRow, numCols);
        }
        else if (!leftClick) {
            interactRightClick(resultSlot, slotCol, slotRow, numCols);
        }
        setItemDecals(glX, glY);
    }

    if (bindedFurnace != nullptr) {
        bindedFurnace->checkToSmelt();
    }
}

void FurnaceUI::interactLeftClick(Item** slots, int slotCol, int slotRow, int numCols)
{
    int index = slotCol + slotRow * numCols;
    
    if (*itemSelected == false && slots[index] != nullptr) {
        *itemSelected = true;
        selectedItem = slots[index];
        slots[index] = nullptr;
    }
    else if (*itemSelected == true && slots[index] == nullptr) {
        *itemSelected = false;
        slots[index] = selectedItem;
        selectedItem = nullptr;
    }
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

void FurnaceUI::interactRightClick(Item** slots, int slotCol, int slotRow, int numCols)
{
    int index = slotCol + slotRow * numCols;
    
    if (*itemSelected == false && slots[index] != nullptr) {
        *itemSelected = true;
        if (slots[index]->getQuantity() > 1) {
            int larger = slots[index]->getQuantity() / 2;
            selectedItem = new Item(slots[index]->getID(), larger);
            bool test = slots[index]->subtractQuantity(larger);
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

void FurnaceUI::updateInventoryPosition() {
    float inventoryWidth = 175.0f * scale;
    float inventoryHeight = 164.0f * scale;
    float centerX = screenWidth / 2;
    float centerY = screenHeight / 2;
    inventoryBottomLeftX = centerX - inventoryWidth / 2;
    inventoryBottomLeftY = centerY - inventoryHeight / 2;
}

SlotRegion FurnaceUI::getClickedSection(float glX, float glY) 
{  
    float slotTransformX;
    float slotTransformY;

    const float slotSpacing = 18.0f * scale;

    float startSlotX;
    float startSlotY;

    int slotCol;
    int slotRow;

    float rangeX;
    float rangeY;

    float numCols;
    float numRows;

    bool rowOffset = false;
    
    // Backpack
    slotTransformX = 8.0f * scale;
    slotTransformY = 62.0f * scale;

    startSlotX = inventoryBottomLeftX + slotTransformX;
    startSlotY = inventoryBottomLeftY + slotTransformY + (1 * slotSpacing);

    slotCol = (glX - startSlotX) / slotSpacing;
    slotRow = (startSlotY - glY) / slotSpacing;

    rangeX = fmod(glX - startSlotX, slotSpacing);
    rangeY = fmod(startSlotY - glY, slotSpacing);

    numCols = 9;
    numRows = 4;
    rowOffset = true;

    bool checkIfInventory = checkRegion(slotCol, slotRow, numCols, numRows, rangeX, rangeY, rowOffset);

    if (checkIfInventory) {
        return SlotRegion::BACKPACK;
    }

    // Smeltable slot
    slotTransformX = 56.0f * scale;
    slotTransformY = 130.0f * scale;

    startSlotX = inventoryBottomLeftX + slotTransformX;
    startSlotY = inventoryBottomLeftY + slotTransformY + (1 * slotSpacing);

    slotCol = (glX - startSlotX) / slotSpacing;
    slotRow = (startSlotY - glY) / slotSpacing;

    rangeX = fmod(glX - startSlotX, slotSpacing);
    rangeY = fmod(startSlotY - glY, slotSpacing);

    numCols = 1;
    numRows = 1;
    rowOffset = false;

    bool checkIfSmeltable = checkRegion(slotCol, slotRow, numCols, numRows, rangeX, rangeY, rowOffset);

    if (checkIfSmeltable) {
        return SlotRegion::SMELTABLE;
    }

    // Fuel slot
    slotTransformX = 56.0f * scale;
    slotTransformY = 94.0f * scale;

    startSlotX = inventoryBottomLeftX + slotTransformX;
    startSlotY = inventoryBottomLeftY + slotTransformY + (1 * slotSpacing);

    slotCol = (glX - startSlotX) / slotSpacing;
    slotRow = (startSlotY - glY) / slotSpacing;

    rangeX = fmod(glX - startSlotX, slotSpacing);
    rangeY = fmod(startSlotY - glY, slotSpacing);

    numCols = 1;
    numRows = 1;
    rowOffset = false;

    bool checkIfFuel = checkRegion(slotCol, slotRow, numCols, numRows, rangeX, rangeY, rowOffset);

    if (checkIfFuel) {
        return SlotRegion::FUEL;
    }

    // Result slot
    slotTransformX = 120.0f * scale;
    slotTransformY = 111.0f * scale;

    startSlotX = inventoryBottomLeftX + slotTransformX;
    startSlotY = inventoryBottomLeftY + slotTransformY + (1 * slotSpacing);

    slotCol = (glX - startSlotX) / slotSpacing;
    slotRow = (startSlotY - glY) / slotSpacing;

    rangeX = fmod(glX - startSlotX, slotSpacing);
    rangeY = fmod(startSlotY - glY, slotSpacing);

    numCols = 1;
    numRows = 1;
    rowOffset = false;

    bool checkIfResult = checkRegion(slotCol, slotRow, numCols, numRows, rangeX, rangeY, rowOffset);

    if (checkIfResult) {
        return SlotRegion::RESULT;
    }

    return SlotRegion::NONE;
}

bool FurnaceUI::checkRegion(int slotCol, int slotRow, float numCols, float numRows, float rangeX, float rangeY, bool rowOffset) {
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

bool FurnaceUI::checkIfSmeltable(ItemID id)
{
    if (id == ItemID::IRON_ORE || id == ItemID::GOLD_ORE || id == ItemID::OAKPLANK_BLOCK || id == ItemID::CLAY_BALL)
    {
        return true;
    }
    return false;
}

bool FurnaceUI::checkIfFuel(ItemID id)
{
    if (id == ItemID::OAKLOG_BLOCK || id == ItemID::COAL)
    {
        return true;
    }
    return false;
}

void FurnaceUI::setFurnaceUI()
{
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    float width = 175.0f * scale;  
    float height = 164.0f * scale;
    
    float atlasSize = 256.0f;

    float iconX = 0.0f;
    float iconY = 0.0f;

    float iconWidth = 175.0f;
    float iconHeight = 164.0f;
    
    float u1 = iconX / atlasSize;
    float v1 = iconY / atlasSize;
    float u2 = (iconX + iconWidth) / atlasSize;
    float v2 = (iconY + iconHeight) / atlasSize;

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

void FurnaceUI::setItemDecals(float glX, float glY)
{
    if (glX != 0 && glY != 0) {
        mouseX = glX;
        mouseY = glY;
    }

    itemDecals.vertices.clear();
    itemDecals.indices.clear();

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    float width  = 16.0f * scale;  
    float height = 16.0f * scale;

    float iconWidth =  16.0f;
    float iconHeight = 16.0f;
    
    float atlasSize = 256.0f;

    float inventoryWidth = 175.0f * scale;  
    float inventoryHeight = 164.0f * scale;

    float inventoryBottomLeftX = centerX - (inventoryWidth/2);
    float inventoryBottomLeftY = centerY - (inventoryHeight/2);

    float inventoryStartX = inventoryBottomLeftX + 8.0f * scale;
    float inventoryStartY = inventoryBottomLeftY + 84.0f * scale;

    float increment = 18.0f * scale;

    Item** backpack = playerInventory.backpack;

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

    inventoryStartX = inventoryBottomLeftX + 56.0f * scale;
    inventoryStartY = inventoryBottomLeftY + 17.0f * scale;

    Item* smeltable = nullptr;
    if (bindedFurnace != nullptr) {
        smeltable = bindedFurnace->smeltable;
    }

    if (smeltable != nullptr) {
        ItemID itemID = smeltable->getID();
        glm::vec2* uvs = Item::getUVCoords(itemID);

        float posX = inventoryStartX;
        float posY = inventoryStartY;

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

    inventoryStartX = inventoryBottomLeftX + 56.0f * scale;
    inventoryStartY = inventoryBottomLeftY + 53.0f * scale;

    Item* fuel = nullptr;
    if (bindedFurnace != nullptr) {
        fuel = bindedFurnace->fuel;
    }

    if (fuel != nullptr) {
        ItemID itemID = fuel->getID();
        glm::vec2* uvs = Item::getUVCoords(itemID);

        float posX = inventoryStartX;
        float posY = inventoryStartY;

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

    
    inventoryStartX = inventoryBottomLeftX + 117.0f * scale;
    inventoryStartY = inventoryBottomLeftY + 35.0f * scale;

    Item* result = nullptr;
    if (bindedFurnace != nullptr) {
        result = bindedFurnace->result;
    }

    if (result != nullptr) {
        float iconX;
        float iconY;

        ItemID itemID = result->getID();
        glm::vec2* uvs = Item::getUVCoords(itemID);

        UIVertex vert1 = { { inventoryStartX, inventoryStartY },   {uvs[0]} };
        UIVertex vert2 = { { inventoryStartX + width, inventoryStartY},   { uvs[1]} };
        UIVertex vert3 = { { inventoryStartX, inventoryStartY + height },   { uvs[2] } };
        UIVertex vert4 = { { inventoryStartX + width, inventoryStartY + height },   { uvs[3] } };

        itemDecals.vertices.insert(itemDecals.vertices.end(), {
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
            { {mouseX, screenHeight - mouseY}, {uvs[0]} },
            { {mouseX + width, screenHeight - mouseY}, {uvs[1]} },
            { {mouseX, screenHeight - (mouseY - height)}, {uvs[2]} },
            { {mouseX + width, screenHeight - (mouseY - height)}, {uvs[3]} }
        });

        GLuint offset = itemDecals.vertices.size() - 4;
        itemDecals.indices.insert(itemDecals.indices.end(), {
            offset + 0, offset + 1, offset + 2,
            offset + 2, offset + 1, offset + 3
        });

    }
    itemDecals.UploadDecalData();
}
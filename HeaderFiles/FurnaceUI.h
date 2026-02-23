#ifndef FURNACE_UI_H
#define FURNACE_UI_H

#include "UIPanel.h"
#include "PlayerInventory.h"
#include "ItemDecals.h"
#include "Crafting.h"
#include "World.h"

#include "imgui/imgui.h"

#include <optional>

class FurnaceUI : public UIPanel {
    public:
        PlayerInventory& playerInventory;
        ItemDecals itemDecals;

        Item* selectedItem;

        bool* itemSelected = nullptr;

        FurnaceUI(PlayerInventory& playerInventory, Texture& inventoryAtlas, Texture& itemAtlas);

        void bindFurnace(Furnace* newFurnace);
        void updateGUI();
        void playerClick(float glX, float glY, bool* itemSelected, bool leftClick);

        bool checkIfSmelting();
        
        void render() override;

        void setItemDecals(float glX, float glY);
        
    private:
        float mouseX, mouseY;

        float scale = 2.0f;
        
        float inventoryBottomLeftX;
        float inventoryBottomLeftY;

        Furnace* bindedFurnace = nullptr;

        SlotRegion getClickedSection(float glX, float glY);
        bool checkRegion(int slotCol, int slotRow, float numCols, float numRows, float rangeX, float rangeY, bool rowOffset);

        void interactLeftClick(Item** slots, int slotCol, int slotRow, int numCols);
        void interactRightClick(Item** slots, int slotCol, int slotRow, int numCols);
        
        bool checkIfFuel(ItemID id);
        bool checkIfSmeltable(ItemID id);

        void updateInventoryPosition();
        void setFurnaceUI();
        void renderInventoryText();
        void addText(float posX, float posY, int quantity);
};

#endif
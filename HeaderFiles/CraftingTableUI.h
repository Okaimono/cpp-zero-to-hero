#ifndef CRAFTING_TABLE_UI
#define CRAFTING_TABLE_UI

#include "UIPanel.h"
#include "PlayerInventory.h"
#include "ItemDecals.h"
#include "Crafting.h"

#include "imgui/imgui.h"

#include <optional>



class CraftingTableUI : public UIPanel {
    public:
        PlayerInventory& playerInventory;
        ItemDecals itemDecals;

        Item* craftingStorage[3][3];
        Item* craftingResult;

        Item* selectedItem;
        
        void checkCraft();

        bool* itemSelected = nullptr;

        CraftingTableUI(PlayerInventory& playerInventory, Texture& inventoryAtlas, Texture& itemAtlas);


        void updateGUI();
        void playerClick(float glX, float glY, bool* itemSelected, bool leftClick);
        void craftItem();
        
        void render() override;
        void flushItem(Item item);

        void setItemDecals(float glX, float glY);
    private:
        float scale = 2.0f;
        float mouseX, mouseY;
        void setCraftingTableUI();
        void renderInventoryText();
        void addText(float posX, float posY, int quantity);

        float inventoryBottomLeftX;
        float inventoryBottomLeftY;

        void updateInventoryPosition();
        SlotRegion getClickedSection(float glX, float glY);
        bool checkRegion(int slotCol, int slotRow, float numCols, float numRows, float rangeX, float rangeY, bool rowOffset);
        void interactLeftClick(Item** slots, int slotCol, int slotRow, int numCols);
        void interactRightClick(Item** slots, int slotCol, int slotRow, int numCols);
        void interactCraftResult(Item** slots, int slotCol, int slotRow);   
};

#endif
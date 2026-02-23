#ifndef INVENTORY_UI
#define INVENTORY_UI

#include "UIPanel.h"
#include "PlayerInventory.h"
#include "ItemDecals.h"
#include "Crafting.h"
#include "imgui/imgui.h"

#include <optional>

class InventoryUI : public UIPanel {
    public:
        PlayerInventory& playerInventory;
        ItemDecals itemDecals;
        ItemDecals selectedItemDecal;

        Item* craftingStorage[2][2];
        Item* craftingResult;

        Item* selectedItem;
        void checkCraft();

        bool* itemSelected = nullptr;

        InventoryUI(PlayerInventory& playerInventory, Texture& inventoryAtlas, Texture& itemAtlas);

        bool clickedInCrafting();
        bool clickedInResult();

        void updateGUI();
        void updateSelectedItem();
        void playerClick(float glX, float glY, bool* itemSelected, bool leftClick);
        void craftItem();
        
        void render() override;
        void flushItem(Item item);

        void setItemDecals(float glX, float glY);

    private:
        float mouseX, mouseY;

        float scale = 2.0f;

        float inventoryBottomLeftX;
        float inventoryBottomLeftY;

        SlotRegion getClickedSection(float glX, float glY);
        bool checkRegion(int slotCol, int slotRow, float numCols, float numRows, float rangeX, float rangeY, bool rowOffset);

        void interactLeftClick(Item** slots, int slotCol, int slotRow, int numCols);
        void interactRightClick(Item** slots, int slotCol, int slotRow, int numCols);
        void interactCraftResult(Item** slots, int slotCol, int slotRow);

        void updateInventoryPosition();
        void setInventoryUI();
        void renderInventoryText();
        void addText(float posX, float posY, int quantity);
};

#endif
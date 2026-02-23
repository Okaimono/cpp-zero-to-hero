#ifndef CHEST_UI
#define CHEST_UI

#include "UIPanel.h"
#include "PlayerInventory.h"
#include "ItemDecals.h"
#include "Crafting.h"
#include "World.h"

#include "imgui/imgui.h"

#include <optional>

class ChestUI : public UIPanel {
    public:
        PlayerInventory& playerInventory;
        ItemDecals itemDecals;

        Item* selectedItem;

        bool* itemSelected = nullptr;

        ChestUI(PlayerInventory& playerInventory, Texture& inventoryAtlas, Texture& itemAtlas);

        void bindBarrel(Barrel* newChest);
        void updateGUI();
        void playerClick(float glX, float glY, bool* itemSelected, bool leftClick);
        
        void render() override;
        void flushItem(Item item);

        void setItemDecals(float glX, float glY);
    private:
        float mouseX, mouseY;

        float scale = 2.0f;

        float inventoryBottomLeftX;
        float inventoryBottomLeftY;
        
        void renderInventoryText();
        void addText(float posX, float posY, int quantity);

        SlotRegion getClickedSection(float glX, float glY);
        bool checkRegion(int slotCol, int slotRow, float numCols, float numRows, float rangeX, float rangeY, bool rowOffset);

        void interactLeftClick(Item** slots, int slotCol, int slotRow, int numCols);
        void interactRightClick(Item** slots, int slotCol, int slotRow, int numCols);
        void interactCraftResult(Item** slots, int slotCol, int slotRow);

        void updateInventoryPosition();
        
        Barrel* bindedBarrel = nullptr;
        void setChestUI();
};

#endif
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <vector>
#include <string>
#include <filesystem>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "PlayerInventory.h"


#include "UIPanel.h"
#include "Crosshair.h"
#include "Hotbar.h"
#include "InventoryUI.h"
#include "CraftingTableUI.h"
#include "ChestUI.h"
#include "World.h"
#include "FurnaceUI.h"

#include <memory>

enum class GUIState {MENU, HUD, INVENTORY, CRAFTING_TABLE, FURNACE, BARREL, CHEST};

class GUIManager
{
    public:
        GUIManager(unsigned int sw, unsigned int sh, PlayerInventory& playerInventory, bool* is);

        GUIState guiState = GUIState::HUD;

        void changeGUIState(GUIState newGUIState);

        void InitializeGUI();

        void playerClick(float glX, float glY, bool leftClick);

        void updateSmeltingInterface();

        void RenderGUI();
        void RenderHUD();
        void RenderInventory();
        void RenderCraftingTable();
        void RenderChest();
        void RenderFurnace();

        void clickOnItem();

        void updateBindedBarrel(Barrel* barrel);
        void updateBindedFurnace(Furnace* furnace);

        void refreshHotbar();
        void refreshAllInventoryUI();
        void refreshInventoryInterface(float glX, float glY);
        void refreshCraftingTableInterface(float glX, float glY);
        void refreshChestInterface(float glX, float glY);
        void refreshFurnaceInterface(float glX, float glY);

        bool* itemSelected = nullptr;
        
    private:
        float screenWidth; 
        float screenHeight;

        std::string parentDir;
        std::string texPath;

        Shader GUIShader;
        

        PlayerInventory& playerInventory;

        // 0 = crosshair, 1 = hotbar, 2 = inventory, 3 = item atlas, 4 = crafting table, 5 = chest, 6 furnace
        std::vector<Texture> textureAtlases;

        // For HUD
        std::vector<std::unique_ptr<UIPanel>> hudPanels;

        // For inventory
        std::vector<std::unique_ptr<UIPanel>> inventoryPanels;
        // For bloccks (why am I SO BAD AT CODING )
        std::vector<std::unique_ptr<UIPanel>> interactBlockUIPanels;

        void addGUIAtlas(std::string atlasName);

        void createHUD();
        void createInventory();
        void createBlockInteractUI();
};

#endif

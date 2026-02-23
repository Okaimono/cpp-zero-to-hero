#include "GUIManager.h"
 
namespace fs = std::filesystem;


// I HAVE ABSOLUTELY NO IDEA WHAT I DID HERE. OH MY GOODNESS. 

GUIManager::GUIManager(unsigned int sw, unsigned int sh, PlayerInventory& inventory, bool* is) 
    : GUIShader("ResourceFiles/Shaders/ui.vert", "ResourceFiles/Shaders/ui.frag"), 
    screenWidth(sw),
    screenHeight(sh), 
    playerInventory(inventory), 
    itemSelected(is)
{
    UIPanel::uiShader = &GUIShader;
    UIPanel::screenWidth = sw;
    UIPanel::screenHeight = sh;

    addGUIAtlas("icons.png");
    addGUIAtlas("hotbar_atlas.png");
    addGUIAtlas("inventory.png");
    addGUIAtlas("game_atlas.png");
    addGUIAtlas("crafting_table.png");
    addGUIAtlas("chest_atlas.png");
    addGUIAtlas("furnace.png");
    
    createHUD();
    createInventory();
    createBlockInteractUI();
}


// Renders the GUI
void GUIManager::RenderGUI() 
{
    if (guiState == GUIState::HUD) {
        RenderHUD();
    }
    else if (guiState == GUIState::INVENTORY) {
        RenderInventory();
    }
    else if (guiState == GUIState::MENU) {
        
    }
    else if (guiState == GUIState::CRAFTING_TABLE) {
        RenderCraftingTable();
    }
    else if (guiState == GUIState::BARREL) {
        RenderChest();
    }
    else if (guiState == GUIState::FURNACE) {
        RenderFurnace();
    }
}

// changes the gui state WHY IS THIS SO CONFUSING AHHH
void GUIManager::changeGUIState(GUIState newGUIState)
{
    guiState = newGUIState;
    if (guiState == GUIState::HUD) {
        refreshHotbar();
    }
    else if (guiState == GUIState::CRAFTING_TABLE) {
        refreshCraftingTableInterface(0.0f, 0.0f);
    }
    else if (guiState == GUIState::BARREL) {
        refreshChestInterface(0.0f, 0.0f);
    }
    else if (guiState == GUIState::FURNACE) {
        refreshFurnaceInterface(0.0f, 0.0f);
    }
}

void GUIManager::updateSmeltingInterface()
{
    bool smelting = (dynamic_cast<FurnaceUI*>(interactBlockUIPanels[2].get()))->checkIfSmelting();
    if (smelting && guiState == GUIState::FURNACE) {
        refreshFurnaceInterface(0.0f, 0.0f);
    } 
}

// Adds GUI Atlases
void GUIManager::addGUIAtlas(std::string fileName) 
{
    // File path
    const std::string parentDir = fs::current_path().string() + "/ResourceFiles/Texture/";
    fs::path filePath = fs::path(parentDir) / fileName;  // Handles path separators

    // Creates texture atlas with file name
    Texture atlas(filePath.string().c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    atlas.texUnit(GUIShader, "tex0", 0);
    // Adds the atlas to the atlas vector
    textureAtlases.push_back(atlas);
}

// creates the hud
void GUIManager::createHUD() 
{
    // PUT ON HEAP
    hudPanels.push_back(std::make_unique<Crosshair>(textureAtlases[0]));
    hudPanels.push_back(std::make_unique<Hotbar>(playerInventory, textureAtlases[1], textureAtlases[3]));
}

// Creates the base inventory UI
void GUIManager::createInventory() 
{
    inventoryPanels.push_back(std::make_unique<InventoryUI>(playerInventory, textureAtlases[2], textureAtlases[3]));
    (dynamic_cast<InventoryUI*>(inventoryPanels[0].get()))->itemSelected = itemSelected;
}

// Creates the base BlockInteract interface
void GUIManager::createBlockInteractUI()
{
    // Adds the furnace gui.
    interactBlockUIPanels.push_back(std::make_unique<CraftingTableUI>(playerInventory, textureAtlases[4], textureAtlases[3]));
    (dynamic_cast<CraftingTableUI*>(interactBlockUIPanels[0].get()))->itemSelected = itemSelected;

    // Adds a chest gui
    interactBlockUIPanels.push_back(std::make_unique<ChestUI>(playerInventory, textureAtlases[5], textureAtlases[3]));
    (dynamic_cast<ChestUI*>(interactBlockUIPanels[1].get()))->itemSelected = itemSelected;

    // Adds a Furnace gui
    interactBlockUIPanels.push_back(std::make_unique<FurnaceUI>(playerInventory, textureAtlases[6], textureAtlases[3]));
    (dynamic_cast<FurnaceUI*>(interactBlockUIPanels[2].get()))->itemSelected = itemSelected;
}

// For when the player clicks
void GUIManager::playerClick(float glX, float glY, bool leftClick) 
{
    if (guiState == GUIState::INVENTORY) 
    {
        dynamic_cast<InventoryUI*>(inventoryPanels[0].get())->playerClick(glX, glY, itemSelected, leftClick);
    } 
    else if (guiState == GUIState::CRAFTING_TABLE) 
    {
        dynamic_cast<CraftingTableUI*>(interactBlockUIPanels[0].get())->playerClick(glX, glY, itemSelected, leftClick);
    }
    else if (guiState == GUIState::BARREL)
    {
        dynamic_cast<ChestUI*>(interactBlockUIPanels[1].get())->playerClick(glX, glY, itemSelected, leftClick);
    }
    else if (guiState == GUIState::FURNACE)
    {
        dynamic_cast<FurnaceUI*>(interactBlockUIPanels[2].get())->playerClick(glX, glY, itemSelected, leftClick);
    }
}

void GUIManager::refreshAllInventoryUI()
{
    switch(guiState) {
        case GUIState::INVENTORY:
            refreshInventoryInterface(0.0f, 0.0f);
            break;
        case GUIState::CRAFTING_TABLE:
            refreshCraftingTableInterface(0.0f, 0.0f);
            break;
        case GUIState::CHEST:
            refreshChestInterface(0.0f, 0.0f);
            break;
        case GUIState::FURNACE:
            refreshFurnaceInterface(0.0f, 0.0f);
            break;
        case GUIState::HUD:
            refreshHotbar();
            break;
    }
}

// Refreshes hotbar
void GUIManager::refreshHotbar() 
{
    Hotbar* hotbar = dynamic_cast<Hotbar*>(hudPanels[1].get());
    hotbar->updateGUI();
}

// Refreshes inventory UI
void GUIManager::refreshInventoryInterface(float glX, float glY) 
{
    (dynamic_cast<InventoryUI*>(inventoryPanels[0].get()))->setItemDecals(glX, glY);
}

// Refreshes crafting table UI
void GUIManager::refreshCraftingTableInterface(float glX, float glY) 
{
    (dynamic_cast<CraftingTableUI*>(interactBlockUIPanels[0].get()))->setItemDecals(glX, glY);
}

// Refreshes chest UI
void GUIManager::refreshChestInterface(float glX, float glY) 
{
    (dynamic_cast<ChestUI*>(interactBlockUIPanels[1].get()))->setItemDecals(glX, glY);
}

void GUIManager::refreshFurnaceInterface(float glX, float glY)
{
    (dynamic_cast<FurnaceUI*>(interactBlockUIPanels[2].get()))->setItemDecals(glX, glY);
}



void GUIManager::updateBindedBarrel(Barrel* barrel)
{
    (dynamic_cast<ChestUI*>(interactBlockUIPanels[1].get()))->bindBarrel(barrel);
}

void GUIManager::updateBindedFurnace(Furnace* furnace)
{
    (dynamic_cast<FurnaceUI*>(interactBlockUIPanels[2].get()))->bindFurnace(furnace);
}


// Renders the hud
void GUIManager::RenderHUD() {
    GUIShader.Activate();
    glDisable(GL_DEPTH_TEST);
    
    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (auto& panel : hudPanels) {
        panel->render();
    }
        
    glDisable(GL_BLEND);  // Disable after GUI
    glEnable(GL_DEPTH_TEST);
}

// Renders the inventory
void GUIManager::RenderInventory() {
    GUIShader.Activate();   
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto& panel : inventoryPanels) {
        panel->render();
    }

    glDisable(GL_BLEND);  // Disable after GUI
    glEnable(GL_DEPTH_TEST);
}

// Renders the crafting table
void GUIManager::RenderCraftingTable() {
    GUIShader.Activate();   
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render crafting table

    interactBlockUIPanels[0]->render();

    glDisable(GL_BLEND);  // Disable after GUI
    glEnable(GL_DEPTH_TEST);
}

void GUIManager::RenderChest()
{
    GUIShader.Activate();   
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render crafting table
    interactBlockUIPanels[1]->render();

    glDisable(GL_BLEND);  // Disable after GUI
    glEnable(GL_DEPTH_TEST);
}

void GUIManager::RenderFurnace()
{
    GUIShader.Activate();   
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render crafting table
    interactBlockUIPanels[2]->render();

    glDisable(GL_BLEND);  // Disable after GUI
    glEnable(GL_DEPTH_TEST);
}
// When clicked on item, swaps item selected
void GUIManager::clickOnItem()
{
    *itemSelected = !(*itemSelected);
}
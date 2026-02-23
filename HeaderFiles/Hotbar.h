#ifndef HOTBAR_H
#define HOTBAR_H

#include "UIPanel.h"
#include "PlayerInventory.h"
#include "ItemDecals.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"  // ← Add this at the top


class Hotbar : public UIPanel {
    public:
        PlayerInventory& playerInventory;
        ItemDecals itemDecals;

        Hotbar(PlayerInventory& playerInventory, Texture& hotbarAtlas, Texture& itemAtlas);
        
        void render() override;
        void updateGUI();
        void generateHotbarGUI();
        void renderHotbarText();

        

    private:
        float scale = 2.0f;
        float hotbarLoc = screenHeight - 10 * scale;

        void addHotbarDecal();
        void addSelectedSlotDecal();
        void addItemDecals();
};

#endif
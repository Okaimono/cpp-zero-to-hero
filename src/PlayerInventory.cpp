#include "PlayerInventory.h"
#include <iostream>

// LOGICAL ERROR: add nullptr
PlayerInventory::PlayerInventory()
{
    for (int i = 0; i < 36; i++) {
        backpack[i] = nullptr;
    }
    //backpack[28] = new Item(ItemID::COAL_ORE);
    //backpack[29] = new Item(ItemID::CRAFTINGTABLE_BLOCK, 25);
}

void PlayerInventory::addSingleItemToInventory(Item* item)
{
    if (item == nullptr) return;
    
    // PASS 1: Try to stack in hotbar (27-35)
    for (int i = 27; i < 36; i++) {
        if (backpack[i] != nullptr && backpack[i]->getID() == item->getID()) {
            int space = backpack[i]->getMaxQuantity() - backpack[i]->getQuantity();
            if (space > 0) {
                int toAdd = std::min(space, item->getQuantity());
                backpack[i]->addQuantity(toAdd);
                item->subtractQuantity(toAdd);
                
                if (item->getQuantity() == 0) {
                    delete item;
                    updateHeldItemDisplay();
                    return;
                }
            }
        }
    }

    // PASS 2: Try to stack in main inventory (0-26)
    for (int i = 0; i < 27; i++) {
        if (backpack[i] != nullptr && backpack[i]->getID() == item->getID()) {
            int space = backpack[i]->getMaxQuantity() - backpack[i]->getQuantity();
            if (space > 0) {
                int toAdd = std::min(space, item->getQuantity());
                backpack[i]->addQuantity(toAdd);
                item->subtractQuantity(toAdd);
                
                if (item->getQuantity() == 0) {
                    delete item;
                    updateHeldItemDisplay();
                    return;
                }
            }
        }
    }

    // PASS 3: Find first empty slot in hotbar
    for (int i = 27; i < 36; i++) {
        if (backpack[i] == nullptr) {
            backpack[i] = item;
            updateHeldItemDisplay();
            return;
        }
    }

    // PASS 4: Find first empty slot in main inventory
    for (int i = 0; i < 27; i++) {
        if (backpack[i] == nullptr) {
            backpack[i] = item;
            updateHeldItemDisplay();
            return;
        }
    }

    // Inventory full
    delete item;
}

void PlayerInventory::changeItemSlot(int i)
{
    this->heldItemIndex = i + 27;
    updateHeldItemDisplay();
}

ItemType PlayerInventory::getHeldItem()
{
    if (backpack[heldItemIndex] == nullptr) {
        return ItemType::NONE;
    }
    return backpack[heldItemIndex]->getItemType();
}
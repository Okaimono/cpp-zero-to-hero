#ifndef PLAYER_INVENTORY_H
#define PLAYER_INVENTORY_H

#include "Item.h"

#include <functional>
#include <optional>
#include <vector>


class PlayerInventory
{ 
    public: 

        Item* backpack[36];

        // Hotbar at index 27
        int heldItemIndex = 27;

        PlayerInventory();

        void addItemToInventory(Item* item);
        void addSingleItemToInventory(Item* item);
        void changeItemSlot(int i);

        ItemType getHeldItem();

        void updateHeldItemDisplay() {
            if (onChange) {
                onChange(getHeldItem());
            }
        }

        std::function<void(ItemType)> onChange; 

};

#endif
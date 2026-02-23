#ifndef CRAFTING_H
#define CRAFTING_H

#include "Item.h"
#include <iostream>

class Crafting {
    public:
        static ItemID recipe;


        template<int N>
        static Item* canCraft(Item* craftingSpace[N][N])
        {
            ItemID topLeft = ItemID::NONE;
            ItemID topRight = ItemID::NONE;
            ItemID bottomLeft = ItemID::NONE;
            ItemID bottomRight = ItemID::NONE;

            // For 2x2 Crafting
            if (craftingSpace[0][0] != nullptr) {
                topLeft = craftingSpace[0][0]->getID();
            }
            if (craftingSpace[0][1] != nullptr) {
                topRight = craftingSpace[0][1]->getID();
            }
            if (craftingSpace[1][0] != nullptr) {
                bottomLeft = craftingSpace[1][0]->getID();
            } 
            if (craftingSpace[1][1] != nullptr) {
                bottomRight = craftingSpace[1][1]->getID();
            }
            
            // === 2x2 RECIPES ===
            
            // Oak Log -> 4 Oak Planks
            if (topLeft == ItemID::OAKLOG_BLOCK &&
                topRight == ItemID::NONE &&
                bottomLeft == ItemID::NONE &&
                bottomRight == ItemID::NONE) 
            {
                recipe = ItemID::OAKPLANK_BLOCK;
                return new Item(ItemID::OAKPLANK_BLOCK, 4);
            }
            
            // Spruce Log -> 4 Spruce Planks
            else if (topLeft == ItemID::SPRUCELOG_BLOCK &&
                topRight == ItemID::NONE &&
                bottomLeft == ItemID::NONE &&
                bottomRight == ItemID::NONE) 
            {
                recipe = ItemID::SPRUCEPLANK_BLOCK;
                return new Item(ItemID::SPRUCEPLANK_BLOCK, 4);
            }
            
            // Birch Log -> 4 Birch Planks
            else if (topLeft == ItemID::BIRCHLOG_BLOCK &&
                topRight == ItemID::NONE &&
                bottomLeft == ItemID::NONE &&
                bottomRight == ItemID::NONE) 
            {
                recipe = ItemID::BIRCHPLANK_BLOCK;
                return new Item(ItemID::BIRCHPLANK_BLOCK, 4);
            }

            // 2 Oak Planks -> 4 Sticks
            else if (topLeft == ItemID::OAKPLANK_BLOCK &&
                topRight == ItemID::NONE &&
                bottomLeft == ItemID::OAKPLANK_BLOCK &&
                bottomRight == ItemID::NONE) 
            {
                recipe = ItemID::STICK;
                return new Item(ItemID::STICK, 4);
            }

            // 4 Oak Planks -> Crafting Table
            else if (topLeft == ItemID::OAKPLANK_BLOCK &&
                topRight == ItemID::OAKPLANK_BLOCK &&
                bottomLeft == ItemID::OAKPLANK_BLOCK &&
                bottomRight == ItemID::OAKPLANK_BLOCK) 
            {
                recipe = ItemID::CRAFTINGTABLE_BLOCK;
                return new Item(ItemID::CRAFTINGTABLE_BLOCK, 1);
            }

            // Coal + Stick -> 4 Torches
            else if (topLeft == ItemID::COAL &&
                topRight == ItemID::NONE &&
                bottomLeft == ItemID::STICK &&
                bottomRight == ItemID::NONE)
            {
                recipe = ItemID::TORCH;
                return new Item(ItemID::TORCH, 4);
            }
            
            // 4 Sand -> Sandstone
            else if (topLeft == ItemID::SAND_BLOCK &&
                topRight == ItemID::SAND_BLOCK &&
                bottomLeft == ItemID::SAND_BLOCK &&
                bottomRight == ItemID::SAND_BLOCK) 
            {
                recipe = ItemID::SANDSTONE_BLOCK;
                return new Item(ItemID::SANDSTONE_BLOCK, 1);
            }
            
            // 4 Brick -> Brick Block
            else if (topLeft == ItemID::BRICK &&
                topRight == ItemID::BRICK &&
                bottomLeft == ItemID::BRICK &&
                bottomRight == ItemID::BRICK) 
            {
                recipe = ItemID::BRICK_BLOCK;
                return new Item(ItemID::BRICK_BLOCK, 1);
            }

            // 3x3 Crafting Grid
            if (N == 3) {
                ItemID topCenter = ItemID::NONE;
                ItemID middleLeft = ItemID::NONE;
                ItemID middleCenter = ItemID::NONE;
                ItemID middleRight = ItemID::NONE;
                ItemID bottomCenter = ItemID::NONE;

                // Load 3x3 grid values
                topLeft = craftingSpace[0][0] ? craftingSpace[0][0]->getID() : ItemID::NONE;
                topCenter = craftingSpace[0][1] ? craftingSpace[0][1]->getID() : ItemID::NONE;
                topRight = craftingSpace[0][2] ? craftingSpace[0][2]->getID() : ItemID::NONE;
                middleLeft = craftingSpace[1][0] ? craftingSpace[1][0]->getID() : ItemID::NONE;
                middleCenter = craftingSpace[1][1] ? craftingSpace[1][1]->getID() : ItemID::NONE;
                middleRight = craftingSpace[1][2] ? craftingSpace[1][2]->getID() : ItemID::NONE;
                bottomLeft = craftingSpace[2][0] ? craftingSpace[2][0]->getID() : ItemID::NONE;
                bottomCenter = craftingSpace[2][1] ? craftingSpace[2][1]->getID() : ItemID::NONE;
                bottomRight = craftingSpace[2][2] ? craftingSpace[2][2]->getID() : ItemID::NONE;

                // === STORAGE BLOCKS ===
                
                // 8 Cobblestone -> Furnace
                if (topLeft == ItemID::COBBLESTONE_BLOCK &&
                    topCenter == ItemID::COBBLESTONE_BLOCK &&
                    topRight == ItemID::COBBLESTONE_BLOCK &&
                    middleLeft == ItemID::COBBLESTONE_BLOCK &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::COBBLESTONE_BLOCK &&
                    bottomLeft == ItemID::COBBLESTONE_BLOCK &&
                    bottomCenter == ItemID::COBBLESTONE_BLOCK &&
                    bottomRight == ItemID::COBBLESTONE_BLOCK)
                {
                    recipe = ItemID::FURNACE_BLOCK;
                    return new Item(ItemID::FURNACE_BLOCK, 1);
                }
                
                // 8 Oak Planks -> Chest/Barrel
                else if (topLeft == ItemID::OAKPLANK_BLOCK &&
                    topCenter == ItemID::OAKPLANK_BLOCK &&
                    topRight == ItemID::OAKPLANK_BLOCK &&
                    middleLeft == ItemID::OAKPLANK_BLOCK &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::OAKPLANK_BLOCK &&
                    bottomLeft == ItemID::OAKPLANK_BLOCK &&
                    bottomCenter == ItemID::OAKPLANK_BLOCK &&
                    bottomRight == ItemID::OAKPLANK_BLOCK)
                {
                    recipe = ItemID::BARREL_BLOCK;
                    return new Item(ItemID::BARREL_BLOCK, 1);
                }
                
                // 9 Iron Bars -> Iron Block
                else if (topLeft == ItemID::IRON_BAR &&
                    topCenter == ItemID::IRON_BAR &&
                    topRight == ItemID::IRON_BAR &&
                    middleLeft == ItemID::IRON_BAR &&
                    middleCenter == ItemID::IRON_BAR &&
                    middleRight == ItemID::IRON_BAR &&
                    bottomLeft == ItemID::IRON_BAR &&
                    bottomCenter == ItemID::IRON_BAR &&
                    bottomRight == ItemID::IRON_BAR)
                {
                    recipe = ItemID::IRON_BLOCK;
                    return new Item(ItemID::IRON_BLOCK, 1);
                }
                
                // 9 Gold Bars -> Gold Block
                else if (topLeft == ItemID::GOLD_BAR &&
                    topCenter == ItemID::GOLD_BAR &&
                    topRight == ItemID::GOLD_BAR &&
                    middleLeft == ItemID::GOLD_BAR &&
                    middleCenter == ItemID::GOLD_BAR &&
                    middleRight == ItemID::GOLD_BAR &&
                    bottomLeft == ItemID::GOLD_BAR &&
                    bottomCenter == ItemID::GOLD_BAR &&
                    bottomRight == ItemID::GOLD_BAR)
                {
                    recipe = ItemID::GOLD_BLOCK;
                    return new Item(ItemID::GOLD_BLOCK, 1);
                }
                
                // 9 Diamonds -> Diamond Block
                else if (topLeft == ItemID::DIAMOND &&
                    topCenter == ItemID::DIAMOND &&
                    topRight == ItemID::DIAMOND &&
                    middleLeft == ItemID::DIAMOND &&
                    middleCenter == ItemID::DIAMOND &&
                    middleRight == ItemID::DIAMOND &&
                    bottomLeft == ItemID::DIAMOND &&
                    bottomCenter == ItemID::DIAMOND &&
                    bottomRight == ItemID::DIAMOND)
                {
                    recipe = ItemID::DIAMOND_BLOCK;
                    return new Item(ItemID::DIAMOND_BLOCK, 1);
                }
                
                // 9 Emeralds -> Emerald Block
                else if (topLeft == ItemID::EMERALD &&
                    topCenter == ItemID::EMERALD &&
                    topRight == ItemID::EMERALD &&
                    middleLeft == ItemID::EMERALD &&
                    middleCenter == ItemID::EMERALD &&
                    middleRight == ItemID::EMERALD &&
                    bottomLeft == ItemID::EMERALD &&
                    bottomCenter == ItemID::EMERALD &&
                    bottomRight == ItemID::EMERALD)
                {
                    recipe = ItemID::EMERALD_BLOCK;
                    return new Item(ItemID::EMERALD_BLOCK, 1);
                }
                
                // 9 Redstone -> Redstone Block
                else if (topLeft == ItemID::REDSTONE &&
                    topCenter == ItemID::REDSTONE &&
                    topRight == ItemID::REDSTONE &&
                    middleLeft == ItemID::REDSTONE &&
                    middleCenter == ItemID::REDSTONE &&
                    middleRight == ItemID::REDSTONE &&
                    bottomLeft == ItemID::REDSTONE &&
                    bottomCenter == ItemID::REDSTONE &&
                    bottomRight == ItemID::REDSTONE)
                {
                    recipe = ItemID::REDSTONE_BLOCK;
                    return new Item(ItemID::REDSTONE_BLOCK, 1);
                }
                
                // === PICKAXES ===
                
                // Wooden Pickaxe
                else if (topLeft == ItemID::OAKPLANK_BLOCK &&
                    topCenter == ItemID::OAKPLANK_BLOCK &&
                    topRight == ItemID::OAKPLANK_BLOCK &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::WOODEN_PICKAXE;
                    return new Item(ItemID::WOODEN_PICKAXE, 1);
                }
                // Stone Pickaxe
                else if (topLeft == ItemID::COBBLESTONE_BLOCK &&
                    topCenter == ItemID::COBBLESTONE_BLOCK &&
                    topRight == ItemID::COBBLESTONE_BLOCK &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::STONE_PICKAXE;
                    return new Item(ItemID::STONE_PICKAXE, 1);
                }
                // Iron Pickaxe
                else if (topLeft == ItemID::IRON_BAR &&
                    topCenter == ItemID::IRON_BAR &&
                    topRight == ItemID::IRON_BAR &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::IRON_PICKAXE;
                    return new Item(ItemID::IRON_PICKAXE, 1);
                }
                // Golden Pickaxe
                else if (topLeft == ItemID::GOLD_BAR &&
                    topCenter == ItemID::GOLD_BAR &&
                    topRight == ItemID::GOLD_BAR &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::GOLDEN_PICKAXE;
                    return new Item(ItemID::GOLDEN_PICKAXE, 1);
                }
                // Diamond Pickaxe
                else if (topLeft == ItemID::DIAMOND &&
                    topCenter == ItemID::DIAMOND &&
                    topRight == ItemID::DIAMOND &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::DIAMOND_PICKAXE;
                    return new Item(ItemID::DIAMOND_PICKAXE, 1);
                }
                // Hallowed Pickaxe
                else if (topLeft == ItemID::HALLOWED_BAR &&
                    topCenter == ItemID::HALLOWED_BAR &&
                    topRight == ItemID::HALLOWED_BAR &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::HALLOWED_PICKAXE;
                    return new Item(ItemID::HALLOWED_PICKAXE, 1);
                }

                // === AXES ===
                
                // Wooden Axe
                else if (topLeft == ItemID::OAKPLANK_BLOCK &&
                    topCenter == ItemID::OAKPLANK_BLOCK &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::OAKPLANK_BLOCK &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::WOODEN_AXE;
                    return new Item(ItemID::WOODEN_AXE, 1);
                }
                // Stone Axe
                else if (topLeft == ItemID::COBBLESTONE_BLOCK &&
                    topCenter == ItemID::COBBLESTONE_BLOCK &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::COBBLESTONE_BLOCK &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::STONE_AXE;
                    return new Item(ItemID::STONE_AXE, 1);
                }
                // Iron Axe
                else if (topLeft == ItemID::IRON_BAR &&
                    topCenter == ItemID::IRON_BAR &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::IRON_BAR &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::IRON_AXE;
                    return new Item(ItemID::IRON_AXE, 1);
                }
                // Golden Axe
                else if (topLeft == ItemID::GOLD_BAR &&
                    topCenter == ItemID::GOLD_BAR &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::GOLD_BAR &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::GOLDEN_AXE;
                    return new Item(ItemID::GOLDEN_AXE, 1);
                }
                // Diamond Axe
                else if (topLeft == ItemID::DIAMOND &&
                    topCenter == ItemID::DIAMOND &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::DIAMOND &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::DIAMOND_AXE;
                    return new Item(ItemID::DIAMOND_AXE, 1);
                }
                // Hallowed Axe
                else if (topLeft == ItemID::HALLOWED_BAR &&
                    topCenter == ItemID::HALLOWED_BAR &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::HALLOWED_BAR &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::HALLOWED_AXE;
                    return new Item(ItemID::HALLOWED_AXE, 1);
                }

                // === SHOVELS ===
                
                // Wooden Shovel
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::OAKPLANK_BLOCK &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::WOODEN_SHOVEL;
                    return new Item(ItemID::WOODEN_SHOVEL, 1);
                }
                // Stone Shovel
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::COBBLESTONE_BLOCK &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::STONE_SHOVEL;
                    return new Item(ItemID::STONE_SHOVEL, 1);
                }
                // Iron Shovel
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::IRON_BAR &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::IRON_SHOVEL;
                    return new Item(ItemID::IRON_SHOVEL, 1);
                }
                // Golden Shovel
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::GOLD_BAR &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::GOLDEN_SHOVEL;
                    return new Item(ItemID::GOLDEN_SHOVEL, 1);
                }
                // Diamond Shovel
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::DIAMOND &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::DIAMOND_SHOVEL;
                    return new Item(ItemID::DIAMOND_SHOVEL, 1);
                }
                // Hallowed Shovel
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::HALLOWED_BAR &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::HALLOWED_SHOVEL;
                    return new Item(ItemID::HALLOWED_SHOVEL, 1);
                }

                // === SWORDS ===
                
                // Wooden Sword
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::OAKPLANK_BLOCK &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::OAKPLANK_BLOCK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::WOODEN_SWORD;
                    return new Item(ItemID::WOODEN_SWORD, 1);
                }
                // Stone Sword
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::COBBLESTONE_BLOCK &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::COBBLESTONE_BLOCK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::STONE_SWORD;
                    return new Item(ItemID::STONE_SWORD, 1);
                }
                // Iron Sword
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::IRON_BAR &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::IRON_BAR &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::IRON_SWORD;
                    return new Item(ItemID::IRON_SWORD, 1);
                }
                // Golden Sword
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::GOLD_BAR &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::GOLD_BAR &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::GOLDEN_SWORD;
                    return new Item(ItemID::GOLDEN_SWORD, 1);
                }
                // Diamond Sword
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::DIAMOND &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::DIAMOND &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::DIAMOND_SWORD;
                    return new Item(ItemID::DIAMOND_SWORD, 1);
                }
                // Hallowed Sword
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::HALLOWED_BAR &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::HALLOWED_BAR &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::HALLOWED_SWORD;
                    return new Item(ItemID::HALLOWED_SWORD, 1);
                }

                // === HOES ===
                
                // Wooden Hoe
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::OAKPLANK_BLOCK &&
                    topRight == ItemID::OAKPLANK_BLOCK &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::WOODEN_HOE;
                    return new Item(ItemID::WOODEN_HOE, 1);
                }
                // Stone Hoe
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::COBBLESTONE_BLOCK &&
                    topRight == ItemID::COBBLESTONE_BLOCK &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::STONE_HOE;
                    return new Item(ItemID::STONE_HOE, 1);
                }
                // Iron Hoe
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::IRON_BAR &&
                    topRight == ItemID::IRON_BAR &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::IRON_HOE;
                    return new Item(ItemID::IRON_HOE, 1);
                }
                // Golden Hoe
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::GOLD_BAR &&
                    topRight == ItemID::GOLD_BAR &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::GOLDEN_HOE;
                    return new Item(ItemID::GOLDEN_HOE, 1);
                }
                // Diamond Hoe
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::DIAMOND &&
                    topRight == ItemID::DIAMOND &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::DIAMOND_HOE;
                    return new Item(ItemID::DIAMOND_HOE, 1);
                }
                // Hallowed Hoe
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::HALLOWED_BAR &&
                    topRight == ItemID::HALLOWED_BAR &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::HALLOWED_HOE;
                    return new Item(ItemID::HALLOWED_HOE, 1);
                }
                
                // === LEATHER ARMOR ===
                
                // Leather Helmet
                else if (topLeft == ItemID::LEATHER &&
                    topCenter == ItemID::LEATHER &&
                    topRight == ItemID::LEATHER &&
                    middleLeft == ItemID::LEATHER &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::LEATHER &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::LEATHER_HELMET;
                    return new Item(ItemID::LEATHER_HELMET, 1);
                }
                // Leather Chestplate
                else if (topLeft == ItemID::LEATHER &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::LEATHER &&
                    middleLeft == ItemID::LEATHER &&
                    middleCenter == ItemID::LEATHER &&
                    middleRight == ItemID::LEATHER &&
                    bottomLeft == ItemID::LEATHER &&
                    bottomCenter == ItemID::LEATHER &&
                    bottomRight == ItemID::LEATHER)
                {
                    recipe = ItemID::LEATHER_BREASTPLATE;
                    return new Item(ItemID::LEATHER_BREASTPLATE, 1);
                }
                // Leather Leggings
                else if (topLeft == ItemID::LEATHER &&
                    topCenter == ItemID::LEATHER &&
                    topRight == ItemID::LEATHER &&
                    middleLeft == ItemID::LEATHER &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::LEATHER &&
                    bottomLeft == ItemID::LEATHER &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::LEATHER)
                {
                    recipe = ItemID::LEATHER_LEGGINGS;
                    return new Item(ItemID::LEATHER_LEGGINGS, 1);
                }
                // Leather Boots
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::LEATHER &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::LEATHER &&
                    bottomLeft == ItemID::LEATHER &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::LEATHER)
                {
                    recipe = ItemID::LEATHER_BOOTS;
                    return new Item(ItemID::LEATHER_BOOTS, 1);
                }
                
                // === IRON ARMOR ===
                
                // Iron Helmet
                else if (topLeft == ItemID::IRON_BAR &&
                    topCenter == ItemID::IRON_BAR &&
                    topRight == ItemID::IRON_BAR &&
                    middleLeft == ItemID::IRON_BAR &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::IRON_BAR &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::IRON_HELMET;
                    return new Item(ItemID::IRON_HELMET, 1);
                }
                // Iron Chestplate
                else if (topLeft == ItemID::IRON_BAR &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::IRON_BAR &&
                    middleLeft == ItemID::IRON_BAR &&
                    middleCenter == ItemID::IRON_BAR &&
                    middleRight == ItemID::IRON_BAR &&
                    bottomLeft == ItemID::IRON_BAR &&
                    bottomCenter == ItemID::IRON_BAR &&
                    bottomRight == ItemID::IRON_BAR)
                {
                    recipe = ItemID::IRON_BREASTPLATE;
                    return new Item(ItemID::IRON_BREASTPLATE, 1);
                }
                // Iron Leggings
                else if (topLeft == ItemID::IRON_BAR &&
                    topCenter == ItemID::IRON_BAR &&
                    topRight == ItemID::IRON_BAR &&
                    middleLeft == ItemID::IRON_BAR &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::IRON_BAR &&
                    bottomLeft == ItemID::IRON_BAR &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::IRON_BAR)
                {
                    recipe = ItemID::IRON_LEGGINGS;
                    return new Item(ItemID::IRON_LEGGINGS, 1);
                }
                // Iron Boots
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::IRON_BAR &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::IRON_BAR &&
                    bottomLeft == ItemID::IRON_BAR &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::IRON_BAR)
                {
                    recipe = ItemID::IRON_BOOTS;
                    return new Item(ItemID::IRON_BOOTS, 1);
                }
                
                // === GOLDEN ARMOR ===
                
                // Golden Helmet
                else if (topLeft == ItemID::GOLD_BAR &&
                    topCenter == ItemID::GOLD_BAR &&
                    topRight == ItemID::GOLD_BAR &&
                    middleLeft == ItemID::GOLD_BAR &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::GOLD_BAR &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::GOLDEN_HELMET;
                    return new Item(ItemID::GOLDEN_HELMET, 1);
                }
                // Golden Chestplate
                else if (topLeft == ItemID::GOLD_BAR &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::GOLD_BAR &&
                    middleLeft == ItemID::GOLD_BAR &&
                    middleCenter == ItemID::GOLD_BAR &&
                    middleRight == ItemID::GOLD_BAR &&
                    bottomLeft == ItemID::GOLD_BAR &&
                    bottomCenter == ItemID::GOLD_BAR &&
                    bottomRight == ItemID::GOLD_BAR)
                {
                    recipe = ItemID::GOLDEN_BREASTPLATE;
                    return new Item(ItemID::GOLDEN_BREASTPLATE, 1);
                }
                // Golden Leggings
                else if (topLeft == ItemID::GOLD_BAR &&
                    topCenter == ItemID::GOLD_BAR &&
                    topRight == ItemID::GOLD_BAR &&
                    middleLeft == ItemID::GOLD_BAR &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::GOLD_BAR &&
                    bottomLeft == ItemID::GOLD_BAR &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::GOLD_BAR)
                {
                    recipe = ItemID::GOLDEN_LEGGINGS;
                    return new Item(ItemID::GOLDEN_LEGGINGS, 1);
                }
                // Golden Boots
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::GOLD_BAR &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::GOLD_BAR &&
                    bottomLeft == ItemID::GOLD_BAR &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::GOLD_BAR)
                {
                    recipe = ItemID::GOLDEN_BOOTS;
                    return new Item(ItemID::GOLDEN_BOOTS, 1);
                }
                
                // === DIAMOND ARMOR ===
                
                // Diamond Helmet
                else if (topLeft == ItemID::DIAMOND &&
                    topCenter == ItemID::DIAMOND &&
                    topRight == ItemID::DIAMOND &&
                    middleLeft == ItemID::DIAMOND &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::DIAMOND &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::DIAMOND_HELMET;
                    return new Item(ItemID::DIAMOND_HELMET, 1);
                }
                // Diamond Chestplate
                else if (topLeft == ItemID::DIAMOND &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::DIAMOND &&
                    middleLeft == ItemID::DIAMOND &&
                    middleCenter == ItemID::DIAMOND &&
                    middleRight == ItemID::DIAMOND &&
                    bottomLeft == ItemID::DIAMOND &&
                    bottomCenter == ItemID::DIAMOND &&
                    bottomRight == ItemID::DIAMOND)
                {
                    recipe = ItemID::DIAMOND_BREASTPLATE;
                    return new Item(ItemID::DIAMOND_BREASTPLATE, 1);
                }
                // Diamond Leggings
                else if (topLeft == ItemID::DIAMOND &&
                    topCenter == ItemID::DIAMOND &&
                    topRight == ItemID::DIAMOND &&
                    middleLeft == ItemID::DIAMOND &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::DIAMOND &&
                    bottomLeft == ItemID::DIAMOND &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::DIAMOND)
                {
                    recipe = ItemID::DIAMOND_LEGGINGS;
                    return new Item(ItemID::DIAMOND_LEGGINGS, 1);
                }
                // Diamond Boots
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::DIAMOND &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::DIAMOND &&
                    bottomLeft == ItemID::DIAMOND &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::DIAMOND)
                {
                    recipe = ItemID::DIAMOND_BOOTS;
                    return new Item(ItemID::DIAMOND_BOOTS, 1);
                }
                
                // === HALLOWED ARMOR ===
                
                // Hallowed Helmet
                else if (topLeft == ItemID::HALLOWED_BAR &&
                    topCenter == ItemID::HALLOWED_BAR &&
                    topRight == ItemID::HALLOWED_BAR &&
                    middleLeft == ItemID::HALLOWED_BAR &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::HALLOWED_BAR &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::HALLOWED_HELMET;
                    return new Item(ItemID::HALLOWED_HELMET, 1);
                }
                // Hallowed Chestplate
                else if (topLeft == ItemID::HALLOWED_BAR &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::HALLOWED_BAR &&
                    middleLeft == ItemID::HALLOWED_BAR &&
                    middleCenter == ItemID::HALLOWED_BAR &&
                    middleRight == ItemID::HALLOWED_BAR &&
                    bottomLeft == ItemID::HALLOWED_BAR &&
                    bottomCenter == ItemID::HALLOWED_BAR &&
                    bottomRight == ItemID::HALLOWED_BAR)
                {
                    recipe = ItemID::HALLOWED_BREASTPLATE;
                    return new Item(ItemID::HALLOWED_BREASTPLATE, 1);
                }
                // Hallowed Leggings
                else if (topLeft == ItemID::HALLOWED_BAR &&
                    topCenter == ItemID::HALLOWED_BAR &&
                    topRight == ItemID::HALLOWED_BAR &&
                    middleLeft == ItemID::HALLOWED_BAR &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::HALLOWED_BAR &&
                    bottomLeft == ItemID::HALLOWED_BAR &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::HALLOWED_BAR)
                {
                    recipe = ItemID::HALLOWED_LEGGINGS;
                    return new Item(ItemID::HALLOWED_LEGGINGS, 1);
                }
                // Hallowed Boots
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::HALLOWED_BAR &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::HALLOWED_BAR &&
                    bottomLeft == ItemID::HALLOWED_BAR &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::HALLOWED_BAR)
                {
                    recipe = ItemID::HALLOWED_BOOTS;
                    return new Item(ItemID::HALLOWED_BOOTS, 1);
                }
                
                // === WOOL (All 16 Colors) ===
                
                // White Wool
                else if (topLeft == ItemID::WHITE_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::WHITE_WOOL;
                    return new Item(ItemID::WHITE_WOOL, 1);
                }
                // Red Wool
                else if (topLeft == ItemID::RED_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::RED_WOOL;
                    return new Item(ItemID::RED_WOOL, 1);
                }
                // Yellow Wool
                else if (topLeft == ItemID::YELLOW_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::YELLOW_WOOL;
                    return new Item(ItemID::YELLOW_WOOL, 1);
                }
                // Blue Wool
                else if (topLeft == ItemID::BLUE_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::BLUE_WOOL;
                    return new Item(ItemID::BLUE_WOOL, 1);
                }
                // Green Wool
                else if (topLeft == ItemID::GREEN_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::GREEN_WOOL;
                    return new Item(ItemID::GREEN_WOOL, 1);
                }
                // Orange Wool
                else if (topLeft == ItemID::ORANGE_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::ORANGE_WOOL;
                    return new Item(ItemID::ORANGE_WOOL, 1);
                }
                // Purple Wool
                else if (topLeft == ItemID::PURPLE_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::PURPLE_WOOL;
                    return new Item(ItemID::PURPLE_WOOL, 1);
                }
                // Pink Wool
                else if (topLeft == ItemID::PINK_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::PINK_WOOL;
                    return new Item(ItemID::PINK_WOOL, 1);
                }
                // Lime Wool
                else if (topLeft == ItemID::LIME_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::LIME_WOOL;
                    return new Item(ItemID::LIME_WOOL, 1);
                }
                // Light Blue Wool
                else if (topLeft == ItemID::LIGHTBLUE_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::LIGHTBLUE_WOOL;
                    return new Item(ItemID::LIGHTBLUE_WOOL, 1);
                }
                // Cyan Wool
                else if (topLeft == ItemID::CYAN_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::CYAN_WOOL;
                    return new Item(ItemID::CYAN_WOOL, 1);
                }
                // Magenta Wool
                else if (topLeft == ItemID::MAGENTA_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::MAGENTA_WOOL;
                    return new Item(ItemID::MAGENTA_WOOL, 1);
                }
                // Gray Wool
                else if (topLeft == ItemID::BROWN_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::GRAY_WOOL;
                    return new Item(ItemID::GRAY_WOOL, 1);
                }
                // Light Gray Wool
                else if (topLeft == ItemID::LIGHTGRAY_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::LIGHTGRAY_WOOL;
                    return new Item(ItemID::LIGHTGRAY_WOOL, 1);
                }
                // Black Wool
                else if (topLeft == ItemID::COAL &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::BLACK_WOOL;
                    return new Item(ItemID::BLACK_WOOL, 1);
                }
                // Brown Wool
                else if (topLeft == ItemID::BROWN_DYE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::STRING &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::BROWN_WOOL;
                    return new Item(ItemID::BROWN_WOOL, 1);
                }
                
                // === MISC ITEMS ===
                
                // Shears (2 Iron Bars diagonal)
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::IRON_BAR &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::IRON_BAR &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::SHEARS;
                    return new Item(ItemID::SHEARS, 1);
                }
                
                // 3 Wheat -> Bread
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::WHEAT &&
                    middleCenter == ItemID::WHEAT &&
                    middleRight == ItemID::WHEAT &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::BREAD;
                    return new Item(ItemID::BREAD, 1);
                }
                
                // 4 Glowstone -> Glowstone Block
                else if (topLeft == ItemID::GLOWSTONE &&
                    topCenter == ItemID::GLOWSTONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::GLOWSTONE &&
                    middleCenter == ItemID::GLOWSTONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::GLOWSTONE_BLOCK;
                    return new Item(ItemID::GLOWSTONE_BLOCK, 1);
                }
                
                // 8 Gold Bars + 1 Apple -> Golden Apple
                else if (topLeft == ItemID::GOLD_BAR &&
                    topCenter == ItemID::GOLD_BAR &&
                    topRight == ItemID::GOLD_BAR &&
                    middleLeft == ItemID::GOLD_BAR &&
                    middleCenter == ItemID::APPLE &&
                    middleRight == ItemID::GOLD_BAR &&
                    bottomLeft == ItemID::GOLD_BAR &&
                    bottomCenter == ItemID::GOLD_BAR &&
                    bottomRight == ItemID::GOLD_BAR)
                {
                    recipe = ItemID::GOLDEN_APPLE;
                    return new Item(ItemID::GOLDEN_APPLE, 1);
                }
                
                // 1 Sugar Cane -> 1 Sugar
                else if (topLeft == ItemID::SUGAR_CANE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::SUGAR;
                    return new Item(ItemID::SUGAR, 1);
                }
                
                // === DYES ===
                
                // Rose -> Red Dye
                else if (topLeft == ItemID::ROSE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::RED_DYE;
                    return new Item(ItemID::RED_DYE, 1);
                }
                
                // Yellow Flower -> Yellow Dye
                else if (topLeft == ItemID::YELLOW_FLOWER &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::YELLOW_DYE;
                    return new Item(ItemID::YELLOW_DYE, 1);
                }
                
                // Lapis -> Blue Dye
                else if (topLeft == ItemID::LAPIS &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::BLUE_DYE;
                    return new Item(ItemID::BLUE_DYE, 1);
                }
                
                // Bone -> 3 Bonemeal (White Dye)
                else if (topLeft == ItemID::BONE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::WHITE_DYE;
                    return new Item(ItemID::WHITE_DYE, 3);
                }
                
                // === WEAPONS ===
                
                // Bow (3 Sticks + 3 String)
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::STICK &&
                    topRight == ItemID::STRING &&
                    middleLeft == ItemID::STICK &&
                    middleCenter == ItemID::NONE &&
                    middleRight == ItemID::STRING &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::STICK &&
                    bottomRight == ItemID::STRING)
                {
                    recipe = ItemID::BOW;
                    return new Item(ItemID::BOW, 1);
                }
                
                // Arrows (Flint + Stick + Feather -> 4 Arrows)
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::FLINT &&
                    topRight == ItemID::NONE &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::NONE &&
                    bottomCenter == ItemID::FEATHER &&
                    bottomRight == ItemID::NONE)
                {
                    recipe = ItemID::ARROW;
                    return new Item(ItemID::ARROW, 4);
                }
                
                // Fishing Rod (3 Sticks + 2 String)
                else if (topLeft == ItemID::NONE &&
                    topCenter == ItemID::NONE &&
                    topRight == ItemID::STICK &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::STICK &&
                    middleRight == ItemID::STRING &&
                    bottomLeft == ItemID::STICK &&
                    bottomCenter == ItemID::NONE &&
                    bottomRight == ItemID::STRING)
                {
                    recipe = ItemID::FISHING_ROD;
                    return new Item(ItemID::FISHING_ROD, 1);
                }
                
                // 5 TNT + 4 Sand -> TNT
                else if (topLeft == ItemID::SAND_BLOCK &&
                    topCenter == ItemID::GUNPOWDER &&
                    topRight == ItemID::SAND_BLOCK &&
                    middleLeft == ItemID::GUNPOWDER &&
                    middleCenter == ItemID::SAND_BLOCK &&
                    middleRight == ItemID::GUNPOWDER &&
                    bottomLeft == ItemID::SAND_BLOCK &&
                    bottomCenter == ItemID::GUNPOWDER &&
                    bottomRight == ItemID::SAND_BLOCK)
                {
                    recipe = ItemID::TNT_BLOCK;
                    return new Item(ItemID::TNT_BLOCK, 1);
                }
                
                // 31 Iron Blocks + 4 Iron Bars -> Anvil
                else if (topLeft == ItemID::IRON_BLOCK &&
                    topCenter == ItemID::IRON_BLOCK &&
                    topRight == ItemID::IRON_BLOCK &&
                    middleLeft == ItemID::NONE &&
                    middleCenter == ItemID::IRON_BAR &&
                    middleRight == ItemID::NONE &&
                    bottomLeft == ItemID::IRON_BAR &&
                    bottomCenter == ItemID::IRON_BAR &&
                    bottomRight == ItemID::IRON_BAR)
                {
                    recipe = ItemID::ANVIL;
                    return new Item(ItemID::ANVIL, 1);
                }
            }
            return nullptr;
        }

        template<int N>
        static void completeCraft(Item* craftingSpace[N][N], Item*& craftingResult, Item*& selectedItem, bool* itemSelected)
        {
            if (*itemSelected == false) {
                *itemSelected = true;
                selectedItem = craftingResult;
                craftingResult = nullptr;
                completeRecipe<N>(craftingSpace);
            } 
            else if (selectedItem->getID() == craftingResult->getID()) {
                if (!(selectedItem->getQuantity() + craftingResult->getQuantity() > selectedItem->getMaxQuantity())) {
                    selectedItem->addQuantity(craftingResult->getQuantity());
                    delete craftingResult;
                    craftingResult = nullptr;
                    completeRecipe<N>(craftingSpace);
                }
            }
        }
        
        template<int N>
        static void completeRecipe(Item* craftingSpace[N][N]) {
            bool isRecipeEmpty;
            
            // 2x2 Recipes
            if (recipe == ItemID::OAKPLANK_BLOCK || recipe == ItemID::SPRUCEPLANK_BLOCK || recipe == ItemID::BIRCHPLANK_BLOCK) {
                isRecipeEmpty = craftingSpace[0][0]->subtractQuantity(1);
                if (isRecipeEmpty) {
                    delete craftingSpace[0][0];
                    craftingSpace[0][0] = nullptr;
                }
            }
            else if (recipe == ItemID::STICK) {
                isRecipeEmpty = craftingSpace[0][0]->subtractQuantity(1);
                if (isRecipeEmpty) {
                    delete craftingSpace[0][0];
                    craftingSpace[0][0] = nullptr;
                }
                isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                if (isRecipeEmpty) {
                    delete craftingSpace[1][0];
                    craftingSpace[1][0] = nullptr;
                }
            }
            else if (recipe == ItemID::CRAFTINGTABLE_BLOCK || recipe == ItemID::SANDSTONE_BLOCK || recipe == ItemID::BRICK_BLOCK) {
                isRecipeEmpty = craftingSpace[0][0]->subtractQuantity(1);
                if (isRecipeEmpty) {
                    delete craftingSpace[0][0];
                    craftingSpace[0][0] = nullptr;
                }
                isRecipeEmpty = craftingSpace[0][1]->subtractQuantity(1);
                if (isRecipeEmpty) {
                    delete craftingSpace[0][1];
                    craftingSpace[0][1] = nullptr;
                }
                isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                if (isRecipeEmpty) {
                    delete craftingSpace[1][0];
                    craftingSpace[1][0] = nullptr;
                }
                isRecipeEmpty = craftingSpace[1][1]->subtractQuantity(1);
                if (isRecipeEmpty) {
                    delete craftingSpace[1][1];
                    craftingSpace[1][1] = nullptr;
                }
            }
            else if (recipe == ItemID::TORCH) {
                isRecipeEmpty = craftingSpace[0][0]->subtractQuantity(1);
                if (isRecipeEmpty) {
                    delete craftingSpace[0][0];
                    craftingSpace[0][0] = nullptr;
                }
                isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                if (isRecipeEmpty) {
                    delete craftingSpace[1][0];
                    craftingSpace[1][0] = nullptr;
                }
            }

            // 3x3 Recipes
            if (N == 3) {
                // Storage blocks (9 items -> 1 block)
                if (recipe == ItemID::IRON_BLOCK || recipe == ItemID::GOLD_BLOCK || 
                    recipe == ItemID::DIAMOND_BLOCK || recipe == ItemID::EMERALD_BLOCK || 
                    recipe == ItemID::REDSTONE_BLOCK) 
                {
                    for (int r = 0; r < 3; r++) {
                        for (int c = 0; c < 3; c++) {
                            isRecipeEmpty = craftingSpace[r][c]->subtractQuantity(1);
                            if (isRecipeEmpty) {
                                delete craftingSpace[r][c];
                                craftingSpace[r][c] = nullptr;
                            }
                        }
                    }
                }
                // Hollow squares (Furnace, Barrel)
                else if (recipe == ItemID::FURNACE_BLOCK || recipe == ItemID::BARREL_BLOCK) {
                    for (int i = 0; i < 3; i++) {
                        isRecipeEmpty = craftingSpace[0][i]->subtractQuantity(1);
                        if (isRecipeEmpty) {
                            delete craftingSpace[0][i];
                            craftingSpace[0][i] = nullptr;
                        }
                    }
                    isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][0];
                        craftingSpace[1][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][2];
                        craftingSpace[1][2] = nullptr;
                    }
                    for (int i = 0; i < 3; i++) {
                        isRecipeEmpty = craftingSpace[2][i]->subtractQuantity(1);
                        if (isRecipeEmpty) {
                            delete craftingSpace[2][i];
                            craftingSpace[2][i] = nullptr;
                        }
                    }
                }
                // All pickaxes
                else if (recipe == ItemID::WOODEN_PICKAXE || recipe == ItemID::STONE_PICKAXE ||
                         recipe == ItemID::IRON_PICKAXE || recipe == ItemID::GOLDEN_PICKAXE ||
                         recipe == ItemID::DIAMOND_PICKAXE || recipe == ItemID::HALLOWED_PICKAXE) 
                {
                    for (int i = 0; i < 3; i++) {
                        isRecipeEmpty = craftingSpace[0][i]->subtractQuantity(1);
                        if (isRecipeEmpty) {
                            delete craftingSpace[0][i];
                            craftingSpace[0][i] = nullptr;
                        }
                    }
                    isRecipeEmpty = craftingSpace[1][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][1];
                        craftingSpace[1][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][1];
                        craftingSpace[2][1] = nullptr;
                    }
                }
                // All axes
                else if (recipe == ItemID::WOODEN_AXE || recipe == ItemID::STONE_AXE ||
                         recipe == ItemID::IRON_AXE || recipe == ItemID::GOLDEN_AXE ||
                         recipe == ItemID::DIAMOND_AXE || recipe == ItemID::HALLOWED_AXE) 
                {
                    isRecipeEmpty = craftingSpace[0][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][0];
                        craftingSpace[0][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[0][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][1];
                        craftingSpace[0][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][0];
                        craftingSpace[1][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][1];
                        craftingSpace[1][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][1];
                        craftingSpace[2][1] = nullptr;
                    }
                }
                // All shovels
                else if (recipe == ItemID::WOODEN_SHOVEL || recipe == ItemID::STONE_SHOVEL ||
                         recipe == ItemID::IRON_SHOVEL || recipe == ItemID::GOLDEN_SHOVEL ||
                         recipe == ItemID::DIAMOND_SHOVEL || recipe == ItemID::HALLOWED_SHOVEL) 
                {
                    isRecipeEmpty = craftingSpace[0][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][1];
                        craftingSpace[0][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][1];
                        craftingSpace[1][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][1];
                        craftingSpace[2][1] = nullptr;
                    }
                }
                // All swords
                else if (recipe == ItemID::WOODEN_SWORD || recipe == ItemID::STONE_SWORD ||
                         recipe == ItemID::IRON_SWORD || recipe == ItemID::GOLDEN_SWORD ||
                         recipe == ItemID::DIAMOND_SWORD || recipe == ItemID::HALLOWED_SWORD) 
                {
                    isRecipeEmpty = craftingSpace[0][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][1];
                        craftingSpace[0][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][1];
                        craftingSpace[1][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][1];
                        craftingSpace[2][1] = nullptr;
                    }
                }
                // All hoes
                else if (recipe == ItemID::WOODEN_HOE || recipe == ItemID::STONE_HOE ||
                         recipe == ItemID::IRON_HOE || recipe == ItemID::GOLDEN_HOE ||
                         recipe == ItemID::DIAMOND_HOE || recipe == ItemID::HALLOWED_HOE) 
                {
                    isRecipeEmpty = craftingSpace[0][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][1];
                        craftingSpace[0][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[0][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][2];
                        craftingSpace[0][2] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][1];
                        craftingSpace[1][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][1];
                        craftingSpace[2][1] = nullptr;
                    }
                }
                // All helmets
                else if (recipe == ItemID::LEATHER_HELMET || recipe == ItemID::IRON_HELMET ||
                         recipe == ItemID::GOLDEN_HELMET || recipe == ItemID::DIAMOND_HELMET ||
                         recipe == ItemID::HALLOWED_HELMET)
                {
                    for (int i = 0; i < 3; i++) {
                        isRecipeEmpty = craftingSpace[0][i]->subtractQuantity(1);
                        if (isRecipeEmpty) {
                            delete craftingSpace[0][i];
                            craftingSpace[0][i] = nullptr;
                        }
                    }
                    isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][0];
                        craftingSpace[1][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][2];
                        craftingSpace[1][2] = nullptr;
                    }
                }
                // All chestplates
                else if (recipe == ItemID::LEATHER_BREASTPLATE || recipe == ItemID::IRON_BREASTPLATE ||
                         recipe == ItemID::GOLDEN_BREASTPLATE || recipe == ItemID::DIAMOND_BREASTPLATE ||
                         recipe == ItemID::HALLOWED_BREASTPLATE)
                {
                    isRecipeEmpty = craftingSpace[0][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][0];
                        craftingSpace[0][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[0][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][2];
                        craftingSpace[0][2] = nullptr;
                    }
                    for (int i = 0; i < 3; i++) {
                        isRecipeEmpty = craftingSpace[1][i]->subtractQuantity(1);
                        if (isRecipeEmpty) {
                            delete craftingSpace[1][i];
                            craftingSpace[1][i] = nullptr;
                        }
                    }
                    for (int i = 0; i < 3; i++) {
                        isRecipeEmpty = craftingSpace[2][i]->subtractQuantity(1);
                        if (isRecipeEmpty) {
                            delete craftingSpace[2][i];
                            craftingSpace[2][i] = nullptr;
                        }
                    }
                }
                // All leggings
                else if (recipe == ItemID::LEATHER_LEGGINGS || recipe == ItemID::IRON_LEGGINGS ||
                         recipe == ItemID::GOLDEN_LEGGINGS || recipe == ItemID::DIAMOND_LEGGINGS ||
                         recipe == ItemID::HALLOWED_LEGGINGS)
                {
                    for (int i = 0; i < 3; i++) {
                        isRecipeEmpty = craftingSpace[0][i]->subtractQuantity(1);
                        if (isRecipeEmpty) {
                            delete craftingSpace[0][i];
                            craftingSpace[0][i] = nullptr;
                        }
                    }
                    isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][0];
                        craftingSpace[1][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][2];
                        craftingSpace[1][2] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][0];
                        craftingSpace[2][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][2];
                        craftingSpace[2][2] = nullptr;
                    }
                }
                // All boots
                else if (recipe == ItemID::LEATHER_BOOTS || recipe == ItemID::IRON_BOOTS ||
                         recipe == ItemID::GOLDEN_BOOTS || recipe == ItemID::DIAMOND_BOOTS ||
                         recipe == ItemID::HALLOWED_BOOTS)
                {
                    isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][0];
                        craftingSpace[1][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][2];
                        craftingSpace[1][2] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][0];
                        craftingSpace[2][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][2];
                        craftingSpace[2][2] = nullptr;
                    }
                }
                // All wool (dye + string)
                else if (recipe == ItemID::WHITE_WOOL || recipe == ItemID::RED_WOOL ||
                         recipe == ItemID::YELLOW_WOOL || recipe == ItemID::BLUE_WOOL ||
                         recipe == ItemID::GREEN_WOOL || recipe == ItemID::ORANGE_WOOL ||
                         recipe == ItemID::PURPLE_WOOL || recipe == ItemID::PINK_WOOL ||
                         recipe == ItemID::LIME_WOOL || recipe == ItemID::LIGHTBLUE_WOOL ||
                         recipe == ItemID::CYAN_WOOL || recipe == ItemID::MAGENTA_WOOL ||
                         recipe == ItemID::GRAY_WOOL || recipe == ItemID::LIGHTGRAY_WOOL ||
                         recipe == ItemID::BLACK_WOOL || recipe == ItemID::BROWN_WOOL)
                {
                    isRecipeEmpty = craftingSpace[0][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][0];
                        craftingSpace[0][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][0];
                        craftingSpace[1][0] = nullptr;
                    }
                }
                // Shears
                else if (recipe == ItemID::SHEARS) {
                    isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][0];
                        craftingSpace[1][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][1];
                        craftingSpace[2][1] = nullptr;
                    }
                }
                // Bread
                else if (recipe == ItemID::BREAD) {
                    for (int i = 0; i < 3; i++) {
                        isRecipeEmpty = craftingSpace[1][i]->subtractQuantity(1);
                        if (isRecipeEmpty) {
                            delete craftingSpace[1][i];
                            craftingSpace[1][i] = nullptr;
                        }
                    }
                }
                // Glowstone Block
                else if (recipe == ItemID::GLOWSTONE_BLOCK) {
                    isRecipeEmpty = craftingSpace[0][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][0];
                        craftingSpace[0][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[0][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][1];
                        craftingSpace[0][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][0];
                        craftingSpace[1][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][1];
                        craftingSpace[1][1] = nullptr;
                    }
                }
                // Golden Apple
                else if (recipe == ItemID::GOLDEN_APPLE) {
                    for (int r = 0; r < 3; r++) {
                        for (int c = 0; c < 3; c++) {
                            isRecipeEmpty = craftingSpace[r][c]->subtractQuantity(1);
                            if (isRecipeEmpty) {
                                delete craftingSpace[r][c];
                                craftingSpace[r][c] = nullptr;
                            }
                        }
                    }
                }
                // Simple 1-item recipes (dyes, sugar)
                else if (recipe == ItemID::SUGAR || recipe == ItemID::RED_DYE || 
                         recipe == ItemID::YELLOW_DYE || recipe == ItemID::BLUE_DYE ||
                         recipe == ItemID::WHITE_DYE)
                {
                    isRecipeEmpty = craftingSpace[0][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][0];
                        craftingSpace[0][0] = nullptr;
                    }
                }
                // Bow
                else if (recipe == ItemID::BOW) {
                    isRecipeEmpty = craftingSpace[0][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][1];
                        craftingSpace[0][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[0][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][2];
                        craftingSpace[0][2] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][0];
                        craftingSpace[1][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][2];
                        craftingSpace[1][2] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][1];
                        craftingSpace[2][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][2];
                        craftingSpace[2][2] = nullptr;
                    }
                }
                // Arrow & Fishing Rod (vertical 3 items)
                else if (recipe == ItemID::ARROW) {
                    isRecipeEmpty = craftingSpace[0][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][1];
                        craftingSpace[0][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][1];
                        craftingSpace[1][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][1];
                        craftingSpace[2][1] = nullptr;
                    }
                }
                // Fishing Rod
                else if (recipe == ItemID::FISHING_ROD) {
                    isRecipeEmpty = craftingSpace[0][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[0][2];
                        craftingSpace[0][2] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][1];
                        craftingSpace[1][1] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[1][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][2];
                        craftingSpace[1][2] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][0]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][0];
                        craftingSpace[2][0] = nullptr;
                    }
                    isRecipeEmpty = craftingSpace[2][2]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[2][2];
                        craftingSpace[2][2] = nullptr;
                    }
                }
                // TNT
                else if (recipe == ItemID::TNT_BLOCK) {
                    for (int r = 0; r < 3; r++) {
                        for (int c = 0; c < 3; c++) {
                            isRecipeEmpty = craftingSpace[r][c]->subtractQuantity(1);
                            if (isRecipeEmpty) {
                                delete craftingSpace[r][c];
                                craftingSpace[r][c] = nullptr;
                            }
                        }
                    }
                }
                // Anvil
                else if (recipe == ItemID::ANVIL) {
                    for (int i = 0; i < 3; i++) {
                        isRecipeEmpty = craftingSpace[0][i]->subtractQuantity(1);
                        if (isRecipeEmpty) {
                            delete craftingSpace[0][i];
                            craftingSpace[0][i] = nullptr;
                        }
                    }
                    isRecipeEmpty = craftingSpace[1][1]->subtractQuantity(1);
                    if (isRecipeEmpty) {
                        delete craftingSpace[1][1];
                        craftingSpace[1][1] = nullptr;
                    }
                    for (int i = 0; i < 3; i++) {
                        isRecipeEmpty = craftingSpace[2][i]->subtractQuantity(1);
                        if (isRecipeEmpty) {
                            delete craftingSpace[2][i];
                            craftingSpace[2][i] = nullptr;
                        }
                    }
                }
            }
        }
};

#endif
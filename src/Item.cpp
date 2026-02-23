#include "Item.h"
#include <iostream>

static const std::unordered_map<ItemID, ItemData> ITEM_DATABASE = {
    
    // ========== WOODEN TOOLS (not placeable, show as items) ==========
    {ItemID::WOODEN_PICKAXE, {ItemType::ITEM, ToolType::PICKAXE, 1, 8.5, 2.0, 59, false}},
    {ItemID::WOODEN_AXE,     {ItemType::ITEM, ToolType::AXE, 1, 0.975, 7.0, 59, false}},
    {ItemID::WOODEN_SHOVEL,  {ItemType::ITEM, ToolType::SHOVEL, 1, 0.91, 2.5, 59, false}},
    {ItemID::WOODEN_SWORD,   {ItemType::ITEM, ToolType::SWORD, 1, 2.0, 4.0, 59, false}},
    {ItemID::WOODEN_HOE,     {ItemType::ITEM, ToolType::HOE, 1, 1.0, 1.0, 59, false}},
    
    // ========== STONE TOOLS ==========
    {ItemID::STONE_PICKAXE,  {ItemType::ITEM, ToolType::PICKAXE, 1, 15.0, 3.0, 131, false}},
    {ItemID::STONE_AXE,      {ItemType::ITEM, ToolType::AXE, 1, 1.95, 9.0, 131, false}},
    {ItemID::STONE_SHOVEL,   {ItemType::ITEM, ToolType::SHOVEL, 1, 1.82, 3.5, 131, false}},
    {ItemID::STONE_SWORD,    {ItemType::ITEM, ToolType::SWORD, 1, 4.0, 5.0, 131, false}},
    {ItemID::STONE_HOE,      {ItemType::ITEM, ToolType::HOE, 1, 2.0, 1.0, 131, false}},
    
    // ========== IRON TOOLS ==========
    {ItemID::IRON_PICKAXE,   {ItemType::ITEM, ToolType::PICKAXE, 1, 22.0, 4.0, 250, false}},
    {ItemID::IRON_AXE,       {ItemType::ITEM, ToolType::AXE, 1, 2.78, 9.0, 250, false}},
    {ItemID::IRON_SHOVEL,    {ItemType::ITEM, ToolType::SHOVEL, 1, 2.43, 4.5, 250, false}},
    {ItemID::IRON_SWORD,     {ItemType::ITEM, ToolType::SWORD, 1, 6.0, 6.0, 250, false}},
    {ItemID::IRON_HOE,       {ItemType::ITEM, ToolType::HOE, 1, 3.0, 1.0, 250, false}},
    {ItemID::IRON_SPADE,     {ItemType::ITEM, ToolType::SHOVEL, 1, 2.43, 4.5, 250, false}}, // Alias for shovel
    
    // ========== GOLDEN TOOLS ==========
    {ItemID::GOLDEN_PICKAXE, {ItemType::ITEM, ToolType::PICKAXE, 1, 85.0, 2.0, 32, false}},
    {ItemID::GOLDEN_AXE,     {ItemType::ITEM, ToolType::AXE, 1, 6.5, 7.0, 32, false}},
    {ItemID::GOLDEN_SHOVEL,  {ItemType::ITEM, ToolType::SHOVEL, 1, 3.65, 2.5, 32, false}},
    {ItemID::GOLDEN_SWORD,   {ItemType::ITEM, ToolType::SWORD, 1, 12.0, 4.0, 32, false}},
    {ItemID::GOLDEN_HOE,     {ItemType::ITEM, ToolType::HOE, 1, 6.0, 1.0, 32, false}},
    
    // ========== DIAMOND TOOLS ==========
    {ItemID::DIAMOND_PICKAXE, {ItemType::ITEM, ToolType::PICKAXE, 1, 25.0, 5.0, 1561, false}},
    {ItemID::DIAMOND_AXE,     {ItemType::ITEM, ToolType::AXE, 1, 3.9, 9.0, 1561, false}},
    {ItemID::DIAMOND_SHOVEL,  {ItemType::ITEM, ToolType::SHOVEL, 1, 2.8125, 5.5, 1561, false}},
    {ItemID::DIAMOND_SWORD,   {ItemType::ITEM, ToolType::SWORD, 1, 8.0, 7.0, 1561, false}},
    {ItemID::DIAMOND_HOE,     {ItemType::ITEM, ToolType::HOE, 1, 4.0, 1.0, 1561, false}},

    // ========== HALLOWED TOOLS (Custom tier) ==========
    {ItemID::HALLOWED_PICKAXE, {ItemType::ITEM, ToolType::PICKAXE, 1, 30.0, 6.0, 3000, false}},
    {ItemID::HALLOWED_AXE,     {ItemType::ITEM, ToolType::AXE, 1, 4.875, 10.0, 3000, false}},
    {ItemID::HALLOWED_SHOVEL,  {ItemType::ITEM, ToolType::SHOVEL, 1, 100.0, 6.0, 3000, false}},
    {ItemID::HALLOWED_SWORD,   {ItemType::ITEM, ToolType::SWORD, 1, 10.0, 8.0, 3000, false}},
    {ItemID::HALLOWED_HOE,     {ItemType::ITEM, ToolType::HOE, 1, 5.0, 2.0, 3000, false}},

    // ========== SPECIAL WEAPONS ==========
    {ItemID::BOW,          {ItemType::ITEM, ToolType::NONE, 1, 1.0, 0.0, 384, false}},
    {ItemID::FISHING_ROD,  {ItemType::ITEM, ToolType::NONE, 1, 1.0, 0.0, 64, false}},
    {ItemID::SHEARS,       {ItemType::ITEM, ToolType::NONE, 1, 1.0, 0.0, 238, false}},
    {ItemID::MORNINGSTAR,  {ItemType::ITEM, ToolType::SWORD, 1, 15.0, 12.0, 5000, false}}, // Custom weapon

    // ========== AMMUNITION ==========
    {ItemID::ARROW,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::STAR_ARROW, {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}}, // Custom arrow

    // ========== LEATHER ARMOR ==========
    {ItemID::LEATHER_HELMET,      {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 1.0, 55, false}},
    {ItemID::LEATHER_BREASTPLATE, {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 3.0, 80, false}},
    {ItemID::LEATHER_LEGGINGS,    {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 2.0, 75, false}},
    {ItemID::LEATHER_BOOTS,       {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 1.0, 65, false}},

    // ========== IRON ARMOR ==========
    {ItemID::IRON_HELMET,      {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 2.0, 165, false}},
    {ItemID::IRON_BREASTPLATE, {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 6.0, 240, false}},
    {ItemID::IRON_LEGGINGS,    {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 5.0, 225, false}},
    {ItemID::IRON_BOOTS,       {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 2.0, 195, false}},

    // ========== GOLDEN ARMOR ==========
    {ItemID::GOLDEN_HELMET,      {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 2.0, 77, false}},
    {ItemID::GOLDEN_BREASTPLATE, {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 5.0, 112, false}},
    {ItemID::GOLDEN_LEGGINGS,    {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 3.0, 105, false}},
    {ItemID::GOLDEN_BOOTS,       {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 1.0, 91, false}},

    // ========== DIAMOND ARMOR ==========
    {ItemID::DIAMOND_HELMET,      {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 3.0, 363, false}},
    {ItemID::DIAMOND_BREASTPLATE, {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 8.0, 528, false}},
    {ItemID::DIAMOND_LEGGINGS,    {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 6.0, 495, false}},
    {ItemID::DIAMOND_BOOTS,       {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 3.0, 429, false}},

    // ========== HALLOWED ARMOR (Custom tier) ==========
    {ItemID::HALLOWED_HELMET,      {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 4.0, 600, false}},
    {ItemID::HALLOWED_BREASTPLATE, {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 9.0, 850, false}},
    {ItemID::HALLOWED_LEGGINGS,    {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 7.0, 800, false}},
    {ItemID::HALLOWED_BOOTS,       {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 4.0, 700, false}},

    // ========== HORSE ARMOR (No durability) ==========
    {ItemID::LEATHER_HORSE_ARMOR, {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 3.0, 0, false}},
    {ItemID::IRON_HORSE_ARMOR,    {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 5.0, 0, false}},
    {ItemID::GOLD_HORSE_ARMOR,    {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 7.0, 0, false}},
    {ItemID::DIAMOND_HORSE_ARMOR, {ItemType::ITEM, ToolType::ARMOR, 1, 1.0, 11.0, 0, false}},

    // ========== TERRAIN BLOCKS (Placeable) ==========
    {ItemID::GRASS_BLOCK,      {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::SNOW_GRASS_BLOCK, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::DIRT_BLOCK,       {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::STONE_BLOCK,      {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::COBBLESTONE_BLOCK,{ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::SAND_BLOCK,       {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::GRAVEL_BLOCK,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::CLAY_BLOCK,       {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::BEDROCK_BLOCK,    {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::OBSIDIAN_BLOCK,   {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::CACTUS_BLOCK,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== WOOD BLOCKS (Logs) ==========
    {ItemID::OAKLOG_BLOCK,    {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::SPRUCELOG_BLOCK, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::BIRCHLOG_BLOCK,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== WOOD BLOCKS (Planks) ==========
    {ItemID::OAKPLANK_BLOCK,    {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::SPRUCEPLANK_BLOCK, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::BIRCHPLANK_BLOCK,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== LEAVES ==========
    {ItemID::OAKLEAVES_BLOCK,    {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::SPRUCELEAVES_BLOCK, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::BIRCHLEAVES_BLOCK,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== SAPLINGS (Placeable) ==========
    {ItemID::OAK_SAPLING,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::BIRCH_SAPLING,   {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::SPRUCE_SAPLING,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::DARKOAK_SAPLING, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::JUNGLE_SAPLING,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== ORE BLOCKS ==========
    {ItemID::COAL_ORE,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::IRON_ORE,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::GOLD_ORE,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::DIAMOND_ORE,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::EMERALD_ORE,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::REDSTONE_ORE, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::LAPIS_ORE,    {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== MINERAL BLOCKS ==========
    {ItemID::DIAMOND_BLOCK,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::EMERALD_BLOCK,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::GOLD_BLOCK,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::IRON_BLOCK,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::REDSTONE_BLOCK, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== WOOL BLOCKS ==========
    {ItemID::WHITE_WOOL,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::BLACK_WOOL,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::BLUE_WOOL,      {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::BROWN_WOOL,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::CYAN_WOOL,      {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::GRAY_WOOL,      {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::GREEN_WOOL,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::LIGHTBLUE_WOOL, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::LIGHTGRAY_WOOL, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::LIME_WOOL,      {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::MAGENTA_WOOL,   {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::ORANGE_WOOL,    {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::PINK_WOOL,      {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::PURPLE_WOOL,    {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::RED_WOOL,       {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::YELLOW_WOOL,    {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== BUILDING BLOCKS ==========
    {ItemID::BRICK_BLOCK,      {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::STONEBRICK_BLOCK, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::SANDSTONE_BLOCK,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::GLASS_BLOCK,      {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::GLOWSTONE_BLOCK,  {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== SPECIAL/INTERACTIVE BLOCKS ==========
    {ItemID::TNT_BLOCK,               {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::MELON_BLOCK,             {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::PUMPKIN_BLOCK,           {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::CRAFTINGTABLE_BLOCK,     {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::FURNACE_BLOCK,           {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::BARREL_BLOCK,            {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::ENCHANTMENT_TABLE_BLOCK, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::ANVIL,                   {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::TORCH,                   {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== FLOWERS (Placeable) ==========
    {ItemID::ROSE,          {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},
    {ItemID::YELLOW_FLOWER, {ItemType::BLOCK, ToolType::NONE, 64, 1.0, 0.0, 0, true}},

    // ========== FLUID BLOCKS (Special - stack 1) ==========
    {ItemID::WATER_BLOCK, {ItemType::BLOCK, ToolType::NONE, 1, 1.0, 0.0, 0, true}},
    {ItemID::LAVA_BLOCK,  {ItemType::BLOCK, ToolType::NONE, 1, 1.0, 0.0, 0, true}},

    // ========== RAW MATERIALS - MINERALS ==========
    {ItemID::DIAMOND,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::EMERALD,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::IRON_BAR,     {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::GOLD_BAR,     {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::COAL,         {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::REDSTONE,     {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::LAPIS,        {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::HALLOWED_BAR, {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::GLOWSTONE,    {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},

    // ========== RAW MATERIALS - NATURAL ==========
    {ItemID::STICK,        {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::FLINT,        {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::LEATHER,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::STRING,       {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::FEATHER,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::GUNPOWDER,    {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::SLIME_BALL,   {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::BONE,         {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::BLAZE_ROD,    {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::BLAZE_POWDER, {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::NETHER_STAR,  {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::CLAY_BALL,    {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::BRICK,        {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::SUGAR,        {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::SUGAR_CANE,   {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::FIRE_CHARGE,  {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::SNOW_BALL,    {ItemType::ITEM, ToolType::NONE, 16, 1.0, 0.0, 0, false}},
    {ItemID::BONEMEAL,     {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},

    // ========== DYES ==========
    {ItemID::WHITE_DYE,     {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::BLUE_DYE,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::BROWN_DYE,     {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::CYAN_DYE,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::GREEN_DYE,     {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::LIGHTBLUE_DYE, {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::LIGHTGRAY_DYE, {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::LIME_DYE,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::MAGENTA_DYE,   {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::ORANGE_DYE,    {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::PINK_DYE,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::PURPLE_DYE,    {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::RED_DYE,       {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::YELLOW_DYE,    {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},

    // ========== SEEDS ==========
    {ItemID::SEEDS,          {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::PUMPKIN_SEEDS,  {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::MELON_SEEDS,    {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::BEETROOT_SEEDS, {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::COCOA_SEEDS,    {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},

    // ========== FOOD - CROPS ==========
    {ItemID::WHEAT,       {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::CARROT,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::POTATO,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::MELON_SLICE, {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::APPLE,       {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::GOLDEN_APPLE,{ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},

    // ========== FOOD - RAW MEAT ==========
    {ItemID::RAW_PORKCHOP, {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::RAW_BEEF,     {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::RAW_CHICKEN,  {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},

    // ========== FOOD - COOKED MEAT ==========
    {ItemID::COOKED_PORKCHOP, {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::COOKED_BEEF,     {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::COOKED_CHICKEN,  {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},

    // ========== FOOD - PREPARED ==========
    {ItemID::BREAD,      {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},
    {ItemID::PUMPKINPIE, {ItemType::ITEM, ToolType::NONE, 64, 1.0, 0.0, 0, false}},

    // ========== ANIMAL PRODUCTS ==========
    {ItemID::EGG,         {ItemType::ITEM, ToolType::NONE, 16, 1.0, 0.0, 0, false}},
    {ItemID::MILK_BUCKET, {ItemType::ITEM, ToolType::NONE, 1, 1.0, 0.0, 0, false}},

    // ========== BUCKETS ==========
    {ItemID::WATER_BUCKET, {ItemType::ITEM, ToolType::NONE, 1, 1.0, 0.0, 0, false}},
    {ItemID::LAVA_BUCKET,  {ItemType::ITEM, ToolType::NONE, 1, 1.0, 0.0, 0, false}},
};

Item::Item(ItemID newID, int newQuantity)
    : id(newID), quantity(newQuantity)
{
    auto it = ITEM_DATABASE.find(newID);
    if (it != ITEM_DATABASE.end()) {
        itemData = it->second;
        maxQuantity = itemData.maxStackSize;  // ← ADD THIS
    } else {
        itemData = {ItemType::ITEM, ToolType::NONE, 64, 0.0f, 0.0f, -1};
        maxQuantity = 64;  // ← ADD THIS
    }
}

ItemType Item::getItemType()
{
    return itemData.itemType;
}

bool Item::subtractDurability()
{
    if (itemData.toolType == ToolType::NONE) {return false;}
    if (itemData.durability - 1 <= 0) {
        return true;
    }
    itemData.durability--;
    return false;
}


void Item::setQuantity(int amount)
{
    quantity = amount;
}

ItemID Item::getID()
{
    return id;
}

int Item::getQuantity()
{   
    return quantity;
}

int Item::getMaxQuantity()
{
    return maxQuantity;
}

bool Item::subtractOne() 
{
    if (quantity - 1 == 0) {
        return true;
    }
    quantity--;

    return false;
}

int Item::addQuantity(int amount) {
    if (quantity + amount > maxQuantity) {
        int remainder = amount - (maxQuantity - quantity);
        quantity = maxQuantity;
        return remainder;
    }
    quantity += amount;
    return 0;
}

bool Item::subtractQuantity(int amount) {

    if (amount >= quantity) {
        quantity = 0;
        return true;
    }
    quantity -= amount;
    return false;
}

bool Item::isPlaceable() {
    return itemData.placeable;
}

bool Item::putOnSlot(Item* slotItem) {
    int slotItemQuantity = slotItem->getQuantity();
    int slotItemMaxQuantity = slotItem->getMaxQuantity();

    if (this->getID() != slotItem->getID()) {
        return false;
    }

    if (slotItemQuantity >= slotItemMaxQuantity) {
        return false;
    }

    int totalQuantity = slotItemQuantity + this->quantity;

    if (totalQuantity > slotItemMaxQuantity) {
        slotItem->setQuantity(slotItemMaxQuantity);  // ✓ actually modify slotItem
        this->quantity = totalQuantity - slotItemMaxQuantity;
        return false;  // still have leftover
    } else {
        slotItem->setQuantity(totalQuantity);  // ✓ actually modify slotItem
        this->quantity = 0;
        return true;  // all transferred
    }
}

void Item::setAtlasUVCoords(glm::vec2* uvs, int xPos, int yPos)
{
    xPos--;
    yPos--;
    static const int atlasWidth = 32;
    static const int atlasHeight = 32;
    // How many blocks there are on the atlas ^
    static const float atlasWidthDist = 1.0 / atlasWidth;
    static const float atlasHeightDist = 1.0 / atlasHeight;

    uvs[0] = {0.0f, 0.0f};                      // bottom-left
    uvs[1] = {atlasWidthDist, 0.0f};            // bottom-right
    uvs[2] = {0.0f, atlasHeightDist};           // top-left
    uvs[3] = {atlasWidthDist, atlasHeightDist}; // top-right

    glm::vec2 bCoords = {xPos * atlasWidthDist, yPos * atlasHeightDist};

    uvs[0] += bCoords;
    uvs[1] += bCoords;
    uvs[2] += bCoords;
    uvs[3] += bCoords;
}

glm::vec2* Item::getUVCoords(ItemID itemID) 
{
    static glm::vec2 uvs[4];

    int blockAtlasWidth = 512;
    int blockAtlasLength = 512;

    float xPos = 1.0f / blockAtlasWidth;
    float yPos = 1.0f / blockAtlasLength;

    uvs[0] = {0.0f, yPos};      // bottom-left
    uvs[1] = {xPos, yPos};      // bottom-right
    uvs[2] = {0.0f, 0.0f};      // top-left
    uvs[3] = {xPos, 0.0f};      // top-right

    if (itemID == ItemID::GRASS_BLOCK) {
        setAtlasUVCoords(uvs, 1, 2);
    }
    // Cobblestone
    else if (itemID == ItemID::COBBLESTONE_BLOCK) {
        setAtlasUVCoords(uvs, 2, 1);
    }
    else if (itemID == ItemID::COAL_ORE)
        setAtlasUVCoords(uvs, 3, 1);

    else if (itemID == ItemID::IRON_ORE)
        setAtlasUVCoords(uvs, 4, 1);

    else if (itemID == ItemID::GOLD_ORE)
        setAtlasUVCoords(uvs, 5, 1);

    else if (itemID == ItemID::EMERALD_ORE)
        setAtlasUVCoords(uvs, 6, 1);

    else if (itemID == ItemID::EMERALD_BLOCK)
        setAtlasUVCoords(uvs, 7, 1);

    else if (itemID == ItemID::DIAMOND_BLOCK)
        setAtlasUVCoords(uvs, 8, 1);

    else if (itemID == ItemID::GOLD_BLOCK)
        setAtlasUVCoords(uvs, 9, 1);

    else if (itemID == ItemID::REDSTONE_BLOCK)
        setAtlasUVCoords(uvs, 10, 1);

    else if (itemID == ItemID::IRON_BLOCK)
        setAtlasUVCoords(uvs, 11, 1);

    else if (itemID == ItemID::STONE_BLOCK) {
        setAtlasUVCoords(uvs, 2, 2);
    }

    else if (itemID == ItemID::DIAMOND_ORE) {
        setAtlasUVCoords(uvs, 3, 2);
    }

    else if (itemID == ItemID::REDSTONE_ORE) {
        setAtlasUVCoords(uvs, 4, 2);
    }
    else if (itemID == ItemID::LAPIS_ORE) {
        setAtlasUVCoords(uvs, 5, 2);
    }
    else if (itemID == ItemID::SNOW_GRASS_BLOCK) {
        setAtlasUVCoords(uvs, 6, 2);
    }
    else if (itemID == ItemID::DIRT_BLOCK) {
        setAtlasUVCoords(uvs, 1, 3);
    }
    else if (itemID == ItemID::OAKPLANK_BLOCK) {
        setAtlasUVCoords(uvs, 2, 3);
    }
    else if (itemID == ItemID::SPRUCEPLANK_BLOCK) {
        setAtlasUVCoords(uvs, 3, 3);
    }
    else if (itemID == ItemID::BIRCHPLANK_BLOCK) {
        setAtlasUVCoords(uvs, 4, 3);
    }
    else if (itemID == ItemID::WATER_BLOCK) {
        setAtlasUVCoords(uvs, 5, 3);
    }
    else if (itemID == ItemID::BEDROCK_BLOCK) {
        setAtlasUVCoords(uvs, 6, 3);
    }
    else if (itemID == ItemID::OAKLOG_BLOCK) {
        setAtlasUVCoords(uvs, 1, 4);
    }
    else if (itemID == ItemID::OAKLEAVES_BLOCK) {
        setAtlasUVCoords(uvs,3, 4);
    }
    else if (itemID == ItemID::SAND_BLOCK) {
        setAtlasUVCoords(uvs, 4, 4);
    }
    else if (itemID == ItemID::LAVA_BLOCK) {
        setAtlasUVCoords(uvs, 5, 4);
    }
    else if (itemID == ItemID::OBSIDIAN_BLOCK) {
        setAtlasUVCoords(uvs, 6, 4);
    }





    else if (itemID == ItemID::GLOWSTONE_BLOCK) {
        setAtlasUVCoords(uvs, 7, 4);
    }
    else if (itemID == ItemID::SPRUCELOG_BLOCK) {
        setAtlasUVCoords(uvs, 1, 5);
    }
    else if (itemID == ItemID::SPRUCELEAVES_BLOCK) {
        setAtlasUVCoords(uvs, 3, 5);
    }
    else if (itemID == ItemID::GLASS_BLOCK) {
        setAtlasUVCoords(uvs, 4, 5);
    }
    else if (itemID == ItemID::BRICK_BLOCK) {
        setAtlasUVCoords(uvs, 5, 5);
    }
    else if (itemID == ItemID::STONEBRICK_BLOCK) {
        setAtlasUVCoords(uvs, 6, 5);
    }
    else if (itemID == ItemID::SANDSTONE_BLOCK) {
        setAtlasUVCoords(uvs, 7, 5);
    }
    else if (itemID == ItemID::BIRCHLOG_BLOCK) {
        setAtlasUVCoords(uvs, 1, 6);
    }
    else if (itemID == ItemID::BIRCHLEAVES_BLOCK) {
        setAtlasUVCoords(uvs, 3, 6);
    } 
    else if (itemID == ItemID::CACTUS_BLOCK) {
        setAtlasUVCoords(uvs, 4, 6);
    }
    else if (itemID == ItemID::GRAVEL_BLOCK) {
        setAtlasUVCoords(uvs, 6, 6);
    }
    else if (itemID == ItemID::CRAFTINGTABLE_BLOCK) {
        setAtlasUVCoords(uvs, 1, 7);
    }
    else if (itemID == ItemID::FURNACE_BLOCK) {
        setAtlasUVCoords(uvs, 1, 8);
    }
    else if (itemID == ItemID::BARREL_BLOCK) {
        setAtlasUVCoords(uvs, 3, 9);
    }



    // Wool
    else if (itemID == ItemID::BLACK_WOOL) {
        setAtlasUVCoords(uvs, 1, 10);
    }
    else if (itemID == ItemID::BLUE_WOOL) {
        setAtlasUVCoords(uvs, 2, 10);
    }
    else if (itemID == ItemID::BROWN_WOOL) {
        setAtlasUVCoords(uvs, 3, 10);
    }
    else if (itemID == ItemID::CYAN_WOOL) {
        setAtlasUVCoords(uvs, 4, 10);
    }
    else if (itemID == ItemID::GRAY_WOOL) {
        setAtlasUVCoords(uvs, 5, 10);
    }
    else if (itemID == ItemID::GREEN_WOOL) {
        setAtlasUVCoords(uvs, 6, 10);
    }
    else if (itemID == ItemID::LIGHTBLUE_WOOL) {
        setAtlasUVCoords(uvs, 1, 11);
    }
    else if (itemID == ItemID::LIGHTGRAY_WOOL) {
        setAtlasUVCoords(uvs, 2, 12);
    }
    else if (itemID == ItemID::MAGENTA_WOOL) {
        setAtlasUVCoords(uvs, 4, 11);
    }
    else if (itemID == ItemID::ORANGE_WOOL) {
        setAtlasUVCoords(uvs, 5, 11);
    }
    else if (itemID == ItemID::LIME_WOOL) {
        setAtlasUVCoords(uvs, 3, 11);
    }
    else if (itemID == ItemID::PINK_WOOL) {
        setAtlasUVCoords(uvs, 6, 11);
    }
    else if (itemID == ItemID::PURPLE_WOOL) {
        setAtlasUVCoords(uvs, 1, 12);
    }
    else if (itemID == ItemID::RED_WOOL) {
        setAtlasUVCoords(uvs, 2, 12);
    }
    else if (itemID == ItemID::WHITE_WOOL) {
        setAtlasUVCoords(uvs, 3, 12);
    }
    else if (itemID == ItemID::YELLOW_WOOL) {
        setAtlasUVCoords(uvs, 4, 12);
    }
    else if (itemID == ItemID::TNT_BLOCK) {
        setAtlasUVCoords(uvs, 1, 13);
    }
    else if (itemID == ItemID::STICK) {
        setAtlasUVCoords(uvs, 5, 22);
    }
    else if (itemID == ItemID::TORCH) {
        setAtlasUVCoords(uvs, 6, 23);
    }






    else if (itemID == ItemID::DIAMOND) {
        setAtlasUVCoords(uvs, 1, 32);
    }
    else if (itemID == ItemID::HALLOWED_BAR) {
        setAtlasUVCoords(uvs, 2, 32);
    }
    else if (itemID == ItemID::GOLD_BAR) {
        setAtlasUVCoords(uvs, 3, 32);
    }
    else if (itemID == ItemID::IRON_BAR) {
        setAtlasUVCoords(uvs, 4, 32);
    }
    else if (itemID == ItemID::REDSTONE) {
        setAtlasUVCoords(uvs, 5, 32);
    }
    else if (itemID == ItemID::LAPIS) {
        setAtlasUVCoords(uvs, 6, 32);
    }
    else if (itemID == ItemID::COAL) {
        setAtlasUVCoords(uvs, 7, 32);
    }
    else if (itemID == ItemID::EMERALD) {
        setAtlasUVCoords(uvs, 5, 31);
    }
    else if (itemID == ItemID::DIAMOND_HELMET) {
        setAtlasUVCoords(uvs, 1, 28);
    }
    else if (itemID == ItemID::DIAMOND_BREASTPLATE) {
        setAtlasUVCoords(uvs, 1, 29);
    }
    else if (itemID == ItemID::DIAMOND_LEGGINGS) {
        setAtlasUVCoords(uvs, 1, 30);
    }
    else if (itemID == ItemID::DIAMOND_BOOTS) {
        setAtlasUVCoords(uvs, 1, 31);
    }
    else if (itemID == ItemID::HALLOWED_HELMET) {
        setAtlasUVCoords(uvs, 2, 28);
    }
    else if (itemID == ItemID::HALLOWED_BREASTPLATE) {
        setAtlasUVCoords(uvs, 2, 29);
    }
    else if (itemID == ItemID::HALLOWED_LEGGINGS) {
        setAtlasUVCoords(uvs, 2, 30);
    }
    else if (itemID == ItemID::HALLOWED_BOOTS) {
        setAtlasUVCoords(uvs, 2, 31);
    }
    else if (itemID == ItemID::GOLDEN_HELMET) {
        setAtlasUVCoords(uvs, 3, 28);
    }
    else if (itemID == ItemID::GOLDEN_BREASTPLATE) {
        setAtlasUVCoords(uvs, 3, 29);
    }
    else if (itemID == ItemID::IRON_LEGGINGS) {
        setAtlasUVCoords(uvs, 3, 30);
    }
    else if (itemID == ItemID::GOLDEN_BOOTS) {
        setAtlasUVCoords(uvs, 3, 31);
    }
    else if (itemID == ItemID::IRON_HELMET) {
        setAtlasUVCoords(uvs, 4, 28);
    }
    else if (itemID == ItemID::IRON_BREASTPLATE) {
        setAtlasUVCoords(uvs, 4, 29);
    }
    else if (itemID == ItemID::IRON_LEGGINGS) {
        setAtlasUVCoords(uvs, 4, 30);
    }
    else if (itemID == ItemID::IRON_BOOTS) {
        setAtlasUVCoords(uvs, 4, 31);
    }




    else if (itemID == ItemID::DIAMOND_SWORD) {
        setAtlasUVCoords(uvs, 1, 27);
    }
    else if (itemID == ItemID::DIAMOND_PICKAXE) {
        setAtlasUVCoords(uvs, 1, 26);
    }
    else if (itemID == ItemID::DIAMOND_AXE) {
        setAtlasUVCoords(uvs, 1, 25);
    }
    else if (itemID == ItemID::DIAMOND_SHOVEL) {
        setAtlasUVCoords(uvs, 1, 24);
    }
    else if (itemID == ItemID::DIAMOND_HOE) {
        setAtlasUVCoords(uvs, 1, 23);
    }




    else if (itemID == ItemID::HALLOWED_SWORD) {
        setAtlasUVCoords(uvs, 2, 27);
    }
    else if (itemID == ItemID::HALLOWED_PICKAXE) {
        setAtlasUVCoords(uvs, 2, 26);
    }
    else if (itemID == ItemID::HALLOWED_AXE) {
        setAtlasUVCoords(uvs, 2, 25);
    }
    else if (itemID == ItemID::HALLOWED_SHOVEL) {
        setAtlasUVCoords(uvs, 2, 24);
    }
    else if (itemID == ItemID::HALLOWED_HOE) {
        setAtlasUVCoords(uvs, 2, 23);
    }




  
    else if (itemID == ItemID::GOLDEN_SWORD) {
        setAtlasUVCoords(uvs, 3, 27);
    }
    else if (itemID == ItemID::GOLDEN_PICKAXE) {
        setAtlasUVCoords(uvs, 3, 26);
    }
    else if (itemID == ItemID::GOLDEN_AXE) {
        setAtlasUVCoords(uvs, 3, 25);
    }
    else if (itemID == ItemID::GOLDEN_SHOVEL) {
        setAtlasUVCoords(uvs, 3, 24);
        
    }
    else if (itemID == ItemID::GOLDEN_HOE) {
        setAtlasUVCoords(uvs, 3, 23);
        
    }


    
    else if (itemID == ItemID::STONE_SWORD) {
        setAtlasUVCoords(uvs, 4, 27);
    }
    else if (itemID == ItemID::STONE_PICKAXE) {
        setAtlasUVCoords(uvs, 4, 26);
    }
    else if (itemID == ItemID::STONE_AXE) {
        setAtlasUVCoords(uvs, 4, 25);
    }
    else if (itemID == ItemID::STONE_SHOVEL) {
        setAtlasUVCoords(uvs, 4, 24);
    }
    else if (itemID == ItemID::STONE_HOE) {
        setAtlasUVCoords(uvs, 4, 23);
        
    }



    
    else if (itemID == ItemID::WOODEN_SWORD) {
        setAtlasUVCoords(uvs, 5, 27);
    }
    else if (itemID == ItemID::WOODEN_PICKAXE) {
        setAtlasUVCoords(uvs, 5, 26);
    }
    else if (itemID == ItemID::WOODEN_AXE) {
        setAtlasUVCoords(uvs, 5, 25);
    }
    else if (itemID == ItemID::WOODEN_SHOVEL) {
        setAtlasUVCoords(uvs, 5, 24);
    }
    else if (itemID == ItemID::WOODEN_HOE) {
        setAtlasUVCoords(uvs, 5, 23);
        
    }




    
    else if (itemID == ItemID::IRON_SWORD) {
        setAtlasUVCoords(uvs, 6, 27);
    }
    else if (itemID == ItemID::IRON_PICKAXE) {
        setAtlasUVCoords(uvs, 6, 26);
    }
    else if (itemID == ItemID::IRON_AXE) {
        setAtlasUVCoords(uvs, 6, 25);
    }
    else if (itemID == ItemID::IRON_SHOVEL) {
        setAtlasUVCoords(uvs, 6, 24);
    }


        
    else if (itemID == ItemID::LEATHER_HORSE_ARMOR) {
        setAtlasUVCoords(uvs, 6, 27);
    }
    else if (itemID == ItemID::IRON_HORSE_ARMOR) {
        setAtlasUVCoords(uvs, 6, 26);
    }
    else if (itemID == ItemID::GOLD_HORSE_ARMOR) {
        setAtlasUVCoords(uvs, 6, 25);
    }
    else if (itemID == ItemID::DIAMOND_HORSE_ARMOR) {
        setAtlasUVCoords(uvs, 6, 24);
    }

    else if (itemID == ItemID::CLAY_BALL) {
        setAtlasUVCoords(uvs, 2, 22);
    }
    else if (itemID == ItemID::BRICK) {
        setAtlasUVCoords(uvs, 9, 20);
    }
    

    return uvs;
}


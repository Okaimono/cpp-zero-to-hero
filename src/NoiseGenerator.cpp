#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator(int seed) {
    // Biome noise - matches your coordX * 0.01f
    biomeNoise.SetSeed(seed);
    biomeNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    biomeNoise.SetFrequency(0.01f);
    
    // Forest height noise - matches your coordX * 0.008f
    forestNoise.SetSeed(seed + 1);
    forestNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    forestNoise.SetFrequency(0.008f);
    
    // Ore noise - matches your veinSize = 0.25f
    oreNoise.SetSeed(seed + 2);
    oreNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    oreNoise.SetFrequency(0.25f);
    
    // Tree density noise - matches your coordX * 0.025f
    treeNoise.SetSeed(seed + 3);
    treeNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    treeNoise.SetFrequency(0.025f);
}

float NoiseGenerator::getBiomeNoise(float x, float z) {
    return biomeNoise.GetNoise(x, z);
}

float NoiseGenerator::getForestNoise(float x, float z) {
    return forestNoise.GetNoise(x, z);
}

float NoiseGenerator::getOreNoise(float x, float y, float z) {
    // Offset to match your original (coordX + 60000, y + 30000, coordZ + 60000)
    return oreNoise.GetNoise(x + 60000.0f, y + 30000.0f, z + 60000.0f);
}

float NoiseGenerator::getTreeNoise(float x, float z) {
    return treeNoise.GetNoise(x, z);
}
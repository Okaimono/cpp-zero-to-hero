#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

#include "FastNoiseLite.h"

class NoiseGenerator {
public:
    explicit NoiseGenerator(int seed);
    
    // Direct replacements for your stb_perlin calls
    float getBiomeNoise(float x, float z);      // frequency 0.01
    float getForestNoise(float x, float z);     // frequency 0.008
    float getOreNoise(float x, float y, float z); // frequency 0.25
    float getTreeNoise(float x, float z);       // frequency 0.025

private:
    FastNoiseLite biomeNoise;
    FastNoiseLite forestNoise;
    FastNoiseLite oreNoise;
    FastNoiseLite treeNoise;
};

#endif
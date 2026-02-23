#include "EntityManager.h"

#include <iostream>
#include <algorithm>
#include <x86intrin.h> // or <intrin.h> on MSVC

#include "Camera.h"
#include "PhysicsEngine.h"
#include "Profiler.h"


EntityManager::EntityManager()
    : creeperTex("ResourceFiles/Texture/mobs/creeper.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE)
{
}

void EntityManager::initializeEntityManager(Camera* cam)
{
    camera = cam;
    shader = camera->getShaderPointer();
    creeperTex.texUnit(*shader, "tex0", 0);

    if (debugCow) {
        glm::vec3 cowSpawn = cam->getPosition() + glm::vec3(0.0f, 1.0f, -2.0f);
        testCow = std::make_unique<Cow>(camera->getShaderPointer(), camera, cowSpawn);
    }
    if (debugCreeper) {
        glm::vec3 creeperSpawn = cam->getPosition() + glm::vec3(0.0f, 1.0f, 10.0f);
        testCreeper = std::make_unique<Creeper>(camera->getShaderPointer(), camera, creeperSpawn + glm::vec3(2.0f));
    }
}

void EntityManager::gameMobTick(float dt, World* world)
{
    for (const auto& mobPtr : creepers) {
        mobPtr->gameTick(world);
    }
    creepers.erase(
        std::remove_if(creepers.begin(), creepers.end(), [](const std::unique_ptr<Creeper>& mobPtr) {
            return mobPtr->destroyCreeper();
        }),
        creepers.end()
    );

    if (debugCow) {
        testCow->gameTick(world);
    }
    if (debugCreeper) {
        // EXPERIMENTAL - Teaching myself optimization
        static unsigned long long cycleSum = 0;
        static double timeSum = 0;
        static int frameCount = 0;
        static const int sampleFrames = 500;

        unsigned long long start = __rdtsc();
        auto timeStart = std::chrono::high_resolution_clock::now();

        testCreeper->gameTick(world);

        unsigned long long end = __rdtsc();
        auto timeEnd = std::chrono::high_resolution_clock::now();
        
        unsigned long long cycles = end - start;
        double microseconds = std::chrono::duration<double, std::micro>(timeEnd - timeStart).count();
        
        cycleSum += cycles;
        timeSum += microseconds;
        frameCount++;

        if (frameCount >= sampleFrames) {
            //std::cout << "Average gameTick(): " << (cycleSum / frameCount) << " cycles, " << (timeSum / frameCount) << " microseconds\n";
            cycleSum = 0;
            timeSum = 0;
            frameCount = 0;
        }
    }
}

void EntityManager::generateMob(glm::vec3 coords) {
    creepers.emplace_back(std::make_unique<Creeper>(camera->getShaderPointer(), camera, coords));
}

void EntityManager::renderMobs()
{
    const glm::mat4& camMatrix = camera->getCamMatrix();
    shader->Activate();

    // RENDER CREEPERS
    creeperTex.Bind();
    for (const auto& mobPtr : creepers) {
        mobPtr->renderCreeper(camMatrix);
    }
    if (debugCreeper) {
        //Profiler::profile("", [&](){
            testCreeper->renderCreeper(camMatrix);
        //});

    }
    if (debugCow) {
        testCow->renderCow();
    }
}
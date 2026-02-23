#include "JobQueue.h"
#include <iostream>
#include <algorithm>


void JobQueue::push(Update update) {
    std::lock_guard<std::mutex> lock(mtx);
    updateQueue.push(update);

    if (currentUpdate == nullptr) {
        currentUpdate = &updateQueue.front();
        for (int i = 0; i < currentUpdate->newChunks.size(); i++) {
            uncompletedChunks.insert(currentUpdate->newChunks[i].coords);
        }
    }
    cv.notify_all();
}

void JobQueue::completeTask(ChunkCoords coords) {
    uncompletedChunks.erase(coords);
}

bool JobQueue::checkIfUncomplete(ChunkCoords coords) {
    return uncompletedChunks.find(coords) != uncompletedChunks.end();
}

void JobQueue::getJob(ChunkJob& job) {
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [&]() {
        return (!updateQueue.empty());
    });

    if (currentUpdate == nullptr) {
        currentUpdate = &updateQueue.front();
        for (int i = 0; i < currentUpdate->newChunks.size(); i++) {
            uncompletedChunks.insert(currentUpdate->newChunks[i].coords);
        }
    }

    std::vector<ChunkJob>& newChunks = currentUpdate->newChunks;
    std::vector<ChunkJob>& newMeshes = currentUpdate->newMeshes;

    if (!newChunks.empty()) {
        job = newChunks[newChunks.size() - 1];
        newChunks.pop_back();
        return;
    }
    else if (uncompletedChunks.empty() && !newMeshes.empty()) {
        job = newMeshes[newMeshes.size() - 1];
        newMeshes.pop_back();
        if (newMeshes.empty()) {
            currentUpdate = nullptr;
            updateQueue.pop();
        }
        return;
    }
    return;
}

// void JobQueue::stop() {
//     int currentGeneration;
//     running = false;
//     std::queue<ChunkJob> clearQueue;

//     std::lock_guard<std::mutex> lock(mtx);
//     chunkQueue = clearQueue;
//     meshQueue = clearQueue;
    
//     cv.notify_all(); // wake all threads
// } 
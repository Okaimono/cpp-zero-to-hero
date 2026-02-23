#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include <utility>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <unordered_set>


using ChunkCoords = std::pair<int, int>;

struct TestPairHash {
    size_t operator()(const std::pair<int, int>& p) const {
        return p.first * 100000 + p.second;
    }
};

enum class JobType {
    GenerateChunkData,
    GenerateChunkMesh
};

struct ChunkJob {
	ChunkCoords coords;
    JobType job;
    int generation;
};

struct Update {
    int generation;
    std::vector<ChunkJob> newChunks;
    std::vector<ChunkJob> newMeshes;
};

class JobQueue {
public:
    void push(Update update);
    void getJob(ChunkJob& job);
    void assignTasks();
    void completeTask(ChunkCoords coords);
    bool checkIfUncomplete(ChunkCoords coords);
    void stop();

public:
    std::unordered_set<ChunkCoords, TestPairHash> uncompletedChunks;
    std::condition_variable cv;
    std::mutex mtx;
    std::queue<Update> updateQueue;

    Update* currentUpdate = nullptr;

};

#endif
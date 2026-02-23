#include "World.h"

#include <Profiler.h>
#include <iostream>
#include <algorithm>

using std::vector;

int seed = 1234123;
int chunkSize = 16;
int chunkHeight = 256;


/*
TODO::

COMPLETELY RESET, START FROM ABSOLUTE COMPLETE SCRATCH.

RESPONSIBILITES:
	1. Create a struct
	2. 
*/


World::World(int rendDist) 
	: renderDist(rendDist), 
	worldGenerator(seed, chunkSize, chunkHeight, this, entityManager), 
	userChunkPosition({0, 0})
{
	for (int i = 0; i < 5; i++) {
		workers.emplace_back(&World::genChunkThread, this, std::ref(chunkJobQueue));
	}
}

void World::initializeEntityManager(Camera* cam)
{
	entityManager.initializeEntityManager(cam);
}

void World::gameTick(float dt)
{
	gameSmeltTick(dt);
	gameMobTick(dt);
}

void World::gameMobTick(float dt)
{
	entityManager.gameMobTick(dt, this);
}

void World::gameSmeltTick(float dt)
{
    for (auto it = worldFurnaces.begin(); it != worldFurnaces.end(); ++it) {
        glm::ivec3 pos = it->first;      // Position
        Furnace& furnace = it->second;   // Furnace reference
		furnace.smelt(dt);
    }
}

void World::renderMobs()
{
	entityManager.renderMobs();
}

void World::renderWorld(const glm::mat4& camMatrix) {
	// BOTTLENECK FOUND: 243,135 microseconds (243 milliseconds)
    while (true) {
        ChunkCoords coords;
        bool hasChunk = false;
        {
            std::lock_guard<std::mutex> completedLock(completedChunkMutex);
            if (completedChunks.empty()) break;
            coords = completedChunks.front();
            completedChunks.pop();
            hasChunk = true;
        }
        if (hasChunk) {
            std::lock_guard<std::mutex> gridLock(worldGridMutex);
            auto it = worldGrid.find(coords);
            if (it != worldGrid.end()) {
                chunkRenderer.uploadChunk(it->first, it->second.getVertices(), it->second.getIndices());
            }
        }
    }
    
    chunkRenderer.render(camMatrix);
    renderMobs();
}

void World::genChunkThread(JobQueue& jobQueue) {
	while (true) {
		ChunkJob job;
		jobQueue.getJob(job);
		completeTask(job);
	}
}

void World::completeTask(const ChunkJob& chunkJob) {
    if (chunkJob.job == JobType::GenerateChunkData) {
		Chunk newChunk(
			chunkJob.coords, 
			chunkSize, 
			chunkHeight, 
			worldGenerator, 
			this
		);
		{
			std::lock_guard<std::mutex> lock(worldGridMutex);
			worldGrid.emplace(chunkJob.coords, std::move(newChunk));
		}
		chunkJobQueue.completeTask(chunkJob.coords);
    }
    else {
		Chunk* chunk = nullptr;
		{
			std::lock_guard<std::mutex> lock(worldGridMutex);
			auto it = worldGrid.find(chunkJob.coords);
			if (it != worldGrid.end()) {
				chunk = &it->second;
			}
		}
		if (chunk) {
			chunk->constructMeshData(chunkJob.generation);
			std::lock_guard<std::mutex> newLock(completedChunkMutex);
			completedChunks.push(chunkJob.coords);
		}
    }
}

static constexpr ChunkCoords offset[4] {
	{0, -1},
	{1, 0},
	{0, 1},
	{-1, 0}
};

void World::resetRenderArea() {
	worldGenerationCount++;

	Update update;
	update.generation = worldGenerationCount;

	std::unordered_set<ChunkCoords, PairHash> newRenderArea;

    for (int dx = -renderDist; dx <= renderDist; dx++) {
        for (int dz = -renderDist; dz <= renderDist; dz++) {
            if ((dx * dx) + (dz * dz) < (renderDist * renderDist)) {
                ChunkCoords coords = {userChunkPosition.first + dx, userChunkPosition.second + dz};
				newRenderArea.insert(coords);
            }
        }
    }

	std::unordered_set<ChunkCoords, PairHash> oldRenderArea(renderArea.begin(), renderArea.end());

	std::vector<ChunkCoords> toAdd;
	for (const auto& coords : newRenderArea) {
		if (oldRenderArea.find(coords) == oldRenderArea.end()) {
			toAdd.push_back(coords);
		} 
	}

	std::vector<ChunkCoords> toRemove;
	for (const auto& coords : oldRenderArea) {
		if (newRenderArea.find(coords) == newRenderArea.end()) {
			toRemove.push_back(coords);
		}
	}

	std::unordered_set<ChunkCoords, PairHash> toUpdate;
	std::unordered_set<ChunkCoords, PairHash> ignore(toAdd.begin(), toAdd.end());
	{
		for (const auto& coords : toAdd) {
			for (int i = 0; i < 4; i++) {
				ChunkCoords neighborCoord = coords;
				neighborCoord.first += offset[i].first;
				neighborCoord.second += offset[i].second;
				if (newRenderArea.find(neighborCoord) != newRenderArea.end() && ignore.find(neighborCoord) == ignore.end()) {
					toUpdate.insert(neighborCoord);
				}
			}
		}
	}

	for (const auto& coords : toRemove) {
		std::lock_guard<std::mutex> lock(worldGridMutex);
		auto it = worldGrid.find(coords);
		if (it != worldGrid.end()) {
			it->second.deleteMeshData();
			chunkRenderer.deleteChunk(it->first);
		}
	}

	for (const auto& coords : toAdd) {
		bool shouldCreate = false;
		{
			std::lock_guard<std::mutex> lock(checkIfExistsMutex);  // ← LOCK!
			if (checkIfExists.find(coords) == checkIfExists.end()) {
				checkIfExists.insert(coords);
				shouldCreate = true;
			}
		}
		
		if (shouldCreate) {
			update.newChunks.push_back({coords, JobType::GenerateChunkData, worldGenerationCount});
		}
		update.newMeshes.push_back({coords, JobType::GenerateChunkMesh, worldGenerationCount});
	}

	for (const auto& coords : toUpdate) {
		auto it = worldGrid.find(coords);
		if (it != worldGrid.end()) {
			it->second.deleteMeshData();
		}
		update.newMeshes.push_back({coords, JobType::GenerateChunkMesh, worldGenerationCount});
	}
	
	chunkJobQueue.push(update);

    renderArea.assign(newRenderArea.begin(), newRenderArea.end());
}

void World::updateChunkMeshAt(ChunkCoords coords)
{
    std::lock_guard<std::mutex> worldLock(worldGridMutex);

    auto it = worldGrid.find(coords);
    if (it != worldGrid.end()) {
		Chunk& chunk = it->second;
		std::lock_guard<std::mutex> chunkLock(*(chunk.chunkDataMutex));
		if (chunk.generatingChunk) 
			chunk.restart = true;
		//else 
			//chunkJobQueue.push({it->first, JobType::GenerateChunkMesh, worldGenerationCount});
    }
}

void World::explodeAtCoords(glm::vec3 center, float radius)
{
	std::vector<ChunkCoords> resetChunks;
	
	for (int dx = -radius; dx <= radius; dx++) {
		for (int dy = -radius; dy <= radius; dy++) {
			for (int dz = -radius; dz <= radius; dz++) {
				if ((dx * dx) + (dy * dy) + (dz * dz) < (radius * radius)) {
					float xCoord = center.x + dx;
					float yCoord = center.y + dy;
					float zCoord = center.z + dz;

					int chunkX = (int)floor(xCoord / 16);
					int chunkZ = (int)floor(zCoord / 16);
					ChunkCoords coords = {chunkX, chunkZ};

					ItemID* block = getBlockAtCoords(glm::vec3(xCoord, yCoord, zCoord));
					if (block) {
						*block = ItemID::NONE;
						if (std::find(resetChunks.begin(), resetChunks.end(), coords) == resetChunks.end()) {
							resetChunks.push_back(coords);
						}
					}
				}
			}
		}
	}
	/*for (ChunkCoords chunkCoord : resetChunks) {
		Chunk* chunk = getChunk(chunkCoord);
		if (chunk) {
			chunk->createMesh();
		}
	}*/
}

void World::initializeWorld()
{
	resetRenderArea();
}

Chunk* World::getChunk(ChunkCoords chunkCoords)
{
	auto it = worldGrid.find(chunkCoords);

	if (it != worldGrid.end()) {
		return &(it->second);
	}
	return nullptr;
}

ItemID* World::getBlockAtCoords(glm::vec3 coords)
{
	//std::cout << "=== getBlockAtCoords called ===" << std::endl;
	//std::cout << "Input coords: (" << coords.x << ", " << coords.y << ", " << coords.z << ")" << std::endl;

	if (coords.y < 0 || coords.y >= 256) {
		return nullptr;
	}

	int chunkX = (int)floor(coords.x / 16);
	int chunkZ = (int)floor(coords.z / 16);
	//std::cout << "Chunk coords: (" << chunkX << ", " << chunkZ << ")" << std::endl;
	
	ChunkCoords blockChunkCoords = {chunkX, chunkZ};

	Chunk* chunk = getChunk(blockChunkCoords);

	if (chunk == nullptr) {
		return nullptr;
	}

	int x = (int)floor(coords.x - (chunkX * 16));
	int y = (int)floor(coords.y);
	int z = (int)floor(coords.z - (chunkZ * 16));
	
	//std::cout << "Block indices: (" << x << ", " << y << ", " << z << ")" << std::endl;

	// Check ALL bounds (including negatives) BEFORE accessing
	if (x < 0 || x >= 16 || y < 0 || y >= 256 || z < 0 || z >= 16) {
		std::cout << x << std::endl;
		std::cout << y << std::endl;
		std::cout << z << std::endl;
		return nullptr;
	}

	//std::cout << "Accessing chunk->blocks[" << x << "][" << y << "][" << z << "]" << std::endl;
	//std::cout << "Block type: " << static_cast<int>(chunk->blocks[x][y][z]) << std::endl;
	
	// NOW it's safe to access
	return &chunk->blocks[x][y][z];
}

void World::addFurnaceAt(glm::vec3 coords)
{
	glm::ivec3 furnacePos = {
		static_cast<int>(std::floor(coords.x)),
		static_cast<int>(std::floor(coords.y)),
		static_cast<int>(std::floor(coords.z))
	};

	worldFurnaces.insert({furnacePos, Furnace()});
}

void World::addBarrelAt(glm::vec3 coords)
{
	glm::ivec3 barrelPos = {
		static_cast<int>(std::floor(coords.x)),
		static_cast<int>(std::floor(coords.y)),
		static_cast<int>(std::floor(coords.z))
	};

	worldBarrels.insert({barrelPos, Barrel()});
};

Barrel* World::getBarrelAt(glm::vec3 coords) 
{
    glm::ivec3 barrelPos = {
        static_cast<int>(std::floor(coords.x)),
        static_cast<int>(std::floor(coords.y)),
        static_cast<int>(std::floor(coords.z))
    };
    
    auto it = worldBarrels.find(barrelPos);
    if (it != worldBarrels.end()) {
        return &(it->second);
    }
    
    return nullptr;
}

Furnace* World::getFurnaceAt(glm::vec3 coords)
{
	glm::ivec3 furnacePos = {
		static_cast<int>(std::floor(coords.x)),
		static_cast<int>(std::floor(coords.y)),
		static_cast<int>(std::floor(coords.z))

	};

	auto it = worldFurnaces.find(furnacePos);
    if (it != worldFurnaces.end()) {
        return &(it->second);
    }

	return nullptr;
}

const std::unordered_map<ChunkCoords, Chunk, PairHash>& World::getWorldGrid()
{
	return worldGrid;
}
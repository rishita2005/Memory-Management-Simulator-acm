#include "../include/cache.h"
#include <iostream>

Cache* L1 = nullptr;
Cache* L2 = nullptr;

// Symbolic penalties (cycles)
static const int L1_PENALTY = 1;
static const int L2_PENALTY = 10;
static const int MEM_PENALTY = 100;

Cache::Cache(std::string n, int slots, int bSize)
    : name(n), numSlots(slots), blockSize(bSize), hits(0), misses(0) {
    tags.assign(numSlots, -1);
}

bool Cache::check(int address) {
    int blockTag = address / blockSize;
    int index = blockTag % numSlots;

    if (tags[index] == blockTag) {
        hits++;
        return true;
    } else {
        misses++;

        // Explicit FIFO replacement
        if (tags[index] != -1 && !fifoQueue.empty()) {
            fifoQueue.pop();
        }
        fifoQueue.push(blockTag);
        tags[index] = blockTag;

        return false;
    }
}

void accessAddress(int pAddr) {
    int totalPenalty = 0;

    std::cout << "Accessing Physical Address " << pAddr << std::endl;

    if (L1 && L1->check(pAddr)) {
        totalPenalty += L1_PENALTY;
        std::cout << "L1 Hit (+" << L1_PENALTY << ")\n";
    } else {
        totalPenalty += L1_PENALTY;
        std::cout << "L1 Miss → Check L2\n";

        if (L2 && L2->check(pAddr)) {
            totalPenalty += L2_PENALTY;
            std::cout << "L2 Hit (+" << L2_PENALTY << ")\n";
        } else {
            totalPenalty += L2_PENALTY + MEM_PENALTY;
            std::cout << "L2 Miss → Main Memory (+" << MEM_PENALTY << ")\n";
        }
    }

    std::cout << "Total Access Penalty: " << totalPenalty << " cycles\n";
}

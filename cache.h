#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <vector>
#include <queue>

class Cache {
public:
    std::string name;
    int numSlots;      // number of cache lines
    int blockSize;     // bytes per block
    int hits, misses;

    std::vector<int> tags;      // stored block tags
    std::queue<int> fifoQueue;  // FIFO replacement

    Cache(std::string n, int slots, int bSize);
    bool check(int address);
};

extern Cache* L1;
extern Cache* L2;

void accessAddress(int pAddr);

#endif

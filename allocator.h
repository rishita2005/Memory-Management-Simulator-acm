#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include <vector>
#include <string>

struct MemoryBlock {
    int id;
    int startAddress;
    int size;
    bool isfree;
};

extern std::vector<MemoryBlock> memoryLayout;
extern int totalMemorySize;
extern int nextProcessId;
extern std::string currentAlgo;

void mallocFirstFit(int requestedSize);
void mallocBestFit(int requestedSize);
void mallocWorstFit(int requestedSize);
void freeBlock(int idToFree);
void showStats();

void mallocBuddy(int size);

#endif
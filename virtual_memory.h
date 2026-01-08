#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H
#include <vector>

struct PageTableEntry {
    int frameNumber;
    bool valid;
};

extern const int PAGE_SIZE;
extern std::vector<PageTableEntry> pageTable;

int translateAddress(int virtualAddress);

#endif
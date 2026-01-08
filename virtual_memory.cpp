#include "../include/virtual_memory.h"
#include <iostream>
#include <queue>

const int PAGE_SIZE = 64;
const int MAX_RAM_PAGES = 4; // Simulated physical limit
std::vector<PageTableEntry> pageTable;
std::queue<int> fifoQueue; 

int translateAddress(int virtualAddress) {
    int pageNum = virtualAddress / PAGE_SIZE;
    int offset = virtualAddress % PAGE_SIZE;

    if (pageNum >= (int)pageTable.size()) {
        std::cout << "Error: Address out of bounds." << std::endl;
        return -1;
    }

    // Handle Page Faults
    if (!pageTable[pageNum].valid) {
        std::cout << "PAGE FAULT! Page " << pageNum << " not in RAM." << std::endl;

        // EXPLICIT FIFO REPLACEMENT
        if (fifoQueue.size() >= MAX_RAM_PAGES) {
            int victim = fifoQueue.front();
            fifoQueue.pop();
            pageTable[victim].valid = false;
            std::cout << "FIFO POLICY: RAM Full. Evicting Oldest Page " << victim << std::endl;
        }

        pageTable[pageNum].valid = true;
        fifoQueue.push(pageNum);
        std::cout << "Page " << pageNum << " loaded into RAM." << std::endl;
    }

    int pAddr = (pageTable[pageNum].frameNumber * PAGE_SIZE) + offset;
    std::cout << "Translation: Virtual " << virtualAddress << " -> Physical " << pAddr << std::endl;
    return pAddr;
}
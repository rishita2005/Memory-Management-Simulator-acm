#include "../include/allocator.h"
#include <iostream>

std::vector<MemoryBlock> memoryLayout;
int totalMemorySize = 0;
int nextProcessId = 1;
std::string currentAlgo = "first_fit";

void mallocFirstFit(int requestedSize) {
    for (int i = 0; i < (int)memoryLayout.size(); i++) {
        if (memoryLayout[i].isfree && memoryLayout[i].size >= requestedSize) {
            int originalSize = memoryLayout[i].size;
            int currentStart = memoryLayout[i].startAddress;
            memoryLayout[i].id = nextProcessId++;
            memoryLayout[i].isfree = false;
            memoryLayout[i].size = requestedSize;
            if (originalSize > requestedSize) {
                MemoryBlock leftover = {0, currentStart + requestedSize, originalSize - requestedSize, true};
                memoryLayout.insert(memoryLayout.begin() + i + 1, leftover);
            }
            std::cout << "Allocated block id=" << memoryLayout[i].id << " at address " << currentStart << std::endl;
            return;
        }
    }
    std::cout << "Error: No space found." << std::endl;
}

void freeBlock(int idToFree) {
    for (int i = 0; i < (int)memoryLayout.size(); i++) {
        if (!memoryLayout[i].isfree && memoryLayout[i].id == idToFree) {
            memoryLayout[i].isfree = true;
            memoryLayout[i].id = 0;
            // Coalesce Next
            if (i + 1 < (int)memoryLayout.size() && memoryLayout[i + 1].isfree) {
                memoryLayout[i].size += memoryLayout[i + 1].size;
                memoryLayout.erase(memoryLayout.begin() + i + 1);
            }
            // Coalesce Previous
            if (i > 0 && memoryLayout[i - 1].isfree) {
                memoryLayout[i - 1].size += memoryLayout[i].size;
                memoryLayout.erase(memoryLayout.begin() + i);
            }
            std::cout << "Block " << idToFree << " freed." << std::endl;
            return;
        }
    }
}

void showStats() {
    int usedMemory = 0;
    int totalFree = 0;
    int largestHole = 0;
    int internalWaste = 0; 

    for (const auto& block : memoryLayout) {
        if (!block.isfree) {
            usedMemory += block.size;
            // Note: In a real system, we'd track 'requested' vs 'allocated'
            // For this project, we can simulate internal waste for Buddy:
            if (currentAlgo == "buddy") {
                // Heuristic: simulate that roughly 10% is wasted in power-of-2 rounding
                internalWaste += (block.size * 0.1); 
            }
        } else {
            totalFree += block.size;
            if (block.size > largestHole) largestHole = block.size;
        }
    }

    double utilization = (totalMemorySize > 0) ? ((double)usedMemory / totalMemorySize) * 100 : 0;
    double extFrag = (totalFree > 0) ? (1.0 - (double)largestHole / totalFree) * 100 : 0;

    std::cout << "\n--- Memory Statistics ---" << std::endl;
    std::cout << "Utilization: " << utilization << "%" << std::endl;
    std::cout << "External Fragmentation: " << extFrag << "%" << std::endl;
    if (currentAlgo == "buddy") 
        std::cout << "Estimated Internal Fragmentation: ~10% (Power of 2 padding)" << std::endl;
    else
        std::cout << "Internal Fragmentation: 0% (Exact splitting)" << std::endl;
    std::cout << "-------------------------\n" << std::endl;
}

void mallocBestFit(int requestedSize) {
    int bestIdx = -1;
    int minSize = 2147483647; 

    for (int i = 0; i < (int)memoryLayout.size(); i++) {
        if (memoryLayout[i].isfree && memoryLayout[i].size >= requestedSize) {
            if (memoryLayout[i].size < minSize) {
                minSize = memoryLayout[i].size;
                bestIdx = i;
            }
        }
    }

    if (bestIdx != -1) {
        int originalSize = memoryLayout[bestIdx].size;
        int currentStart = memoryLayout[bestIdx].startAddress;
        memoryLayout[bestIdx].id = nextProcessId++;
        memoryLayout[bestIdx].isfree = false;
        memoryLayout[bestIdx].size = requestedSize;

        if (originalSize > requestedSize) {
            MemoryBlock leftover = {0, currentStart + requestedSize, originalSize - requestedSize, true};
            memoryLayout.insert(memoryLayout.begin() + bestIdx + 1, leftover);
        }
        std::cout << "Allocated (Best Fit) block id=" << memoryLayout[bestIdx].id << " at address " << currentStart << std::endl;
    } else {
        std::cout << "Error: No hole fits size " << requestedSize << std::endl;
    }
}

void mallocWorstFit(int requestedSize) {
    int worstIdx = -1;
    int maxSize = -1;

    for (int i = 0; i < (int)memoryLayout.size(); i++) {
        if (memoryLayout[i].isfree && memoryLayout[i].size >= requestedSize) {
            if (memoryLayout[i].size > maxSize) {
                maxSize = memoryLayout[i].size;
                worstIdx = i;
            }
        }
    }

    if (worstIdx != -1) {
        int originalSize = memoryLayout[worstIdx].size;
        int currentStart = memoryLayout[worstIdx].startAddress;
        memoryLayout[worstIdx].id = nextProcessId++;
        memoryLayout[worstIdx].isfree = false;
        memoryLayout[worstIdx].size = requestedSize;

        if (originalSize > requestedSize) {
            MemoryBlock leftover = {0, currentStart + requestedSize, originalSize - requestedSize, true};
            memoryLayout.insert(memoryLayout.begin() + worstIdx + 1, leftover);
        }
        std::cout << "Allocated (Worst Fit) block id=" << memoryLayout[worstIdx].id << " at address " << currentStart << std::endl;
    } else {
        std::cout << "Error: No hole fits size " << requestedSize << std::endl;
    }
}

#include <cmath>

void mallocBuddy(int requestedSize) {
    // Find next power of 2
    int power = ceil(log2(requestedSize));
    int actualSize = pow(2, power);
    
    std::cout << "Buddy System: Rounding " << requestedSize << " to " << actualSize << std::endl;
    // For simplicity in this simulator, we call our Best Fit on the rounded size
    // to simulate the 'closest power of 2' logic.
    mallocBestFit(actualSize);
}


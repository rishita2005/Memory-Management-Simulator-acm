#include <iostream>
#include <vector>
#include <string>
#include "include/allocator.h"
#include "include/cache.h"
#include "include/virtual_memory.h"

using namespace std;

void dumpMemory() {
    cout << "\n--- Memory Map ---" << endl;
    for (const auto& block : memoryLayout) {
        string status = block.isfree ? "FREE" : "USED (ID: " + to_string(block.id) + ")";
        cout << "[" << block.startAddress << " - "
             << (block.startAddress + block.size - 1) << "] "
             << status << endl;
    }
    cout << "------------------\n" << endl;
}

int main() {
    cout << "Memory Management Simulator Active" << endl;
    string cmd;

    while (true) {
        cout << "> ";
        if (!(cin >> cmd)) break;

        if (cmd == "init") {
            cin >> totalMemorySize;
            memoryLayout.clear();
            nextProcessId = 1;
            memoryLayout.push_back({0, 0, totalMemorySize, true});
            cout << "Memory: " << totalMemorySize << " bytes." << endl;
        }
        else if (cmd == "malloc") {
            int size; cin >> size;
            if (currentAlgo == "first_fit") mallocFirstFit(size);
            else if (currentAlgo == "best_fit") mallocBestFit(size);
            else if (currentAlgo == "worst_fit") mallocWorstFit(size);
            else if (currentAlgo == "buddy") mallocBuddy(size);
        }
        else if (cmd == "free") {
            int id; cin >> id;
            freeBlock(id);
        }
        else if (cmd == "set") {
            string type, algo; cin >> type >> algo;
            if (type == "allocator") {
                currentAlgo = algo;
                cout << "Policy updated to: " << algo << endl;
            }
        }

       
        else if (cmd == "init_cache") {
            int l1Slots, l1Block, l2Slots, l2Block;
            cin >> l1Slots >> l1Block >> l2Slots >> l2Block;

            L1 = new Cache("L1", l1Slots, l1Block);
            L2 = new Cache("L2", l2Slots, l2Block);

            cout << "L1 Cache: " << l1Slots
                 << " slots, Block size " << l1Block << endl;
            cout << "L2 Cache: " << l2Slots
                 << " slots, Block size " << l2Block << endl;
        }

        else if (cmd == "init_vm") {
            int pages; cin >> pages;
            pageTable.clear();
            for (int i = 0; i < pages; i++)
                pageTable.push_back({i + 2, false});
            cout << "Virtual Memory ready." << endl;
        }
        else if (cmd == "v_access") {
            int vAddr; cin >> vAddr;
            int pAddr = translateAddress(vAddr);
            if (pAddr != -1)
                accessAddress(pAddr);
        }
        else if (cmd == "report" || cmd == "stats") {
            showStats();

            cout << "--- Cache Statistics ---" << endl;
            if (L1) {
                double r = (L1->hits + L1->misses > 0)
                           ? (double)L1->hits / (L1->hits + L1->misses) * 100
                           : 0;
                cout << "L1 Hit Ratio: " << r
                     << "% (" << L1->hits << " hits, "
                     << L1->misses << " misses)" << endl;
            }
            if (L2) {
                double r = (L2->hits + L2->misses > 0)
                           ? (double)L2->hits / (L2->hits + L2->misses) * 100
                           : 0;
                cout << "L2 Hit Ratio: " << r
                     << "% (" << L2->hits << " hits, "
                     << L2->misses << " misses)" << endl;
            }
            cout << "------------------------\n" << endl;
        }
        else if (cmd == "dump") {
            dumpMemory();
        }
        else if (cmd == "exit") {
            break;
        }
    }
    return 0;
}

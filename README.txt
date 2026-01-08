
MEMORY MANAGEMENT SIMULATOR - PROJECT README


1. PROJECT OVERVIEW
This program simulates how a computer manages memory. It handles everything from dividing up RAM and using a high-speed cache to translating virtual 
addresses using a paging system.


2. CORE FEATURES
* RAM Management: You can initialize the total memory size and choose between First Fit, Best Fit, Worst Fit, or the Buddy System for 
  allocation.

* Cleaning Up: When you free a process, the system automatically merges adjacent free blocks to prevent fragmentation.

* Two-Level Cache: Includes L1 and L2 caches. If data is missing in L1, the system automatically checks L2 before fetching from RAM.

* Virtual Memory: Implements a page table with 64-byte pages to translate virtual addresses into physical ones.

* Smart Reporting: The system tracks memory usage, internal/external fragmentation, cache hit/miss rates, and page faults.


3. HOW TO BUILD AND RUN

To compile the project using the Makefile:
1. Open your terminal.
2. Type: make

To compile it manually:
1. Use the command: 
    g++ main.cpp src/allocator.cpp src/cache.cpp src/virtual_memory.cpp -Iinclude -o memsim

To start the simulator:
1. Run: ./memsim (or memsim.exe on Windows)


3. FULL SYSTEM STRESS TEST (STEP-BY-STEP)
Follow these commands to test every feature created:

>init 1024
>init_vm 5
>init_cache 4 8
>set allocator buddy
>malloc 70 (Verify rounding to 128)
>v_access 0 (Verify Page Fault/L1 Miss)
>v_access 0 (Verify L1 Hit)
>v_access 64, v_access 128, v_access 192, v_access 256 (Verify FIFO Eviction of Page 0)
>free 1
>dump (Verify coalescing)
>report (Verify final statistics)

5. POLICIES USED
* Cache: Uses Direct-Mapping for placement.
* Page Replacement: Uses the FIFO (First-In, First-Out) rule. When RAM is full, the oldest page is evicted to make room for the new one.

Thank you!

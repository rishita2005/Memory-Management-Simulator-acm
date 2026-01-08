all:
		g++ main.cpp src/allocator.cpp src/cache.cpp src/virtual_memory.cpp -Iinclude -o memsim

clean:
		rm -f memsim
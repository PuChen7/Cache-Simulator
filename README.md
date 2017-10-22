# Cache-Simulator
The program simulates cache structure to evaluate different configurations of caches. 
The program takes an input trace file that contains multiples of lines. Each line corresponds to a memory address. 
It should be able to determie if the input trace is a hit or miss, and take corresponding actions.

# Detail Specifications 
* The program only simulates only one level cache. (L1)
* Replacement Algorithm: First In First Out (FIFO).
* Memory address are 48-bit
* Type of cache
    First Type:
        tag - index - block
    Second Type:
        index - tag - block

# Input Trace File Specification
The trace file is a .txt file which contains multiples of lines. Each line corresponds to a memory address.

The first column reports program counter when this particular memory access occurred. 

Second column lists whether the memory access is a read (R) or a write (W) operation. 

The last column reports the actual 48-bit memory address that has been accessed by the program.

* Sample trace file:

0x804ae19: W 0x9cb32e0
0x804ae1c: R 0x9cb32e4
0x804ae1c: W 0x9cb32e4
0x804ae10: R 0xbf8ef498
0x804ae16: R 0xbf8ef49c

# Running the simulator
* The program take the following parameters:
...`./cacheSimulator <cache size> <associativity> <block size> <trace file>`
* < cachesize > is the total size of the cache in bytes. This number will be a power of 2.
* < associativity > is one of: 
...– direct - simulate a direct mapped cache. 
...– assoc - simulate a fully associative cache. 
...– assoc:n - simulate an n − way associative cache. n will be a power of 2.
* < blocksize > is a power of 2 integer that specifies the size of the cache block in bytes.
* < tracefile > is the name of the trace file.

# Output Sample

$./tracefile 32 assoc:2 4 trace1.txt

cache A

Memory reads: 173

Memory writes: 334

Cache hits: 827 

Cache misses: 173 

cache B

Memory reads: 667 

Memory writes: 334 

Cache hits: 333 

Cache misses: 667

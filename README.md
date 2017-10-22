# Cache-Simulator
The program simulates cache structure to evaluate different configurations of caches. 
The program takes an input trace file that contains multiples of lines. Each line corresponds to a memory address. 
It should be able to determie if the input trace is a hit or miss, and take corresponding actions.

# Detail Specifications 
* The program only simulates only one level cache. (L1)
* Replacement Algorithm: First In First Out (FIFO).
* Memory address are 48-bit
* Type of cache  
&nbsp;&nbsp;&nbsp;First Type:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;tag - index - block  
&nbsp;&nbsp;&nbsp;Second Type:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;index - tag - block  

# Input Trace File Specification
The trace file is a .txt file which contains multiples of lines. Each line corresponds to a memory address.  
The first column reports program counter when this particular memory access occurred.  
Second column lists whether the memory access is a read (R) or a write (W) operation.  
The last column reports the actual 48-bit memory address that has been accessed by the program.

* Sample trace file:
<img src = "https://github.com/PuChen7/Cache-Simulator/blob/master/images/tracefile.jpeg" width="30%" height="30%">

# Running the simulator
* The program take the following parameters:  
&nbsp;&nbsp;&nbsp;`./cacheSimulator <cache size> <associativity> <block size> <trace file>`
* < cachesize > is the total size of the cache in bytes. This number will be a power of 2.
* < associativity > is one of: 
&nbsp;&nbsp;&nbsp;– direct - simulate a direct mapped cache. 
&nbsp;&nbsp;&nbsp;– assoc - simulate a fully associative cache. 
&nbsp;&nbsp;&nbsp;– assoc:n - simulate an n − way associative cache. n will be a power of 2.
* < blocksize > is a power of 2 integer that specifies the size of the cache block in bytes.
* < tracefile > is the name of the trace file.

# Output Sample
<img src = "https://github.com/PuChen7/Cache-Simulator/blob/master/images/output.jpeg" width="30%" height="30%">




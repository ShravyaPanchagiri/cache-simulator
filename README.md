
# High-Performance Userspace Page Cache Simulator (Modern C++)

A high-performance, deterministic userspace simulation of an Operating System Page Cache utilizing a Least Recently Used (LRU) eviction architecture. This project models low-level virtual memory management constraints within a strictly bounded resource space, executing all core operations in deterministic  $O(1)$ constant time.

## Architectural Design & Time Complexity
To avoid the linear overhead ($O(N)$) of searching through sequential arrays, this engine couples two complementary data structures to decouple lookup latency from cache capacity:

1. **Recency Tracking Queue ('std::list'):** A doubly linked list acting as the recency tracking mechanics. Most Recently Used (MRU) nodes are kept at the head, while Least Recently Used (LRU) nodes reside at the tail.
2. **Instant Lookup Engine ('std::unordered_map'):** A hash table pairing unique page IDs directly to their respective iterator locations within the tracking queue, guaranteeing $O(1)$ search mechanics.

### Hardware & OS Primitives Implemented:
* **Resource Acquisition Is Initialization (RAII):** Completely eliminates manual allocation risks (`malloc`/`free`) by leveraging modern smart pointers ('std::unique_ptr').Heap allocations are strictly bound to container lifecycles.
* **Zero-Copy Data Transfer:** Utilizes C++ move semantics ('std::move') to transfer data payloads across interface boundaries, matching the zero-copy design principles of performance-critical network stacks.
* **Deterministic Eviction:** Capacity breaches instantly isolate and reclaim the oldest memory frames at the tail of the layout queue without latency spikes.
* **Zero-Allocation Cache Hits:** Leverages std::list::splice to rearrange structural nodes on a cache hit via internal pointer manipulation, completely bypassing the memory allocator during hot-path execution.


## Toolchain & Compilation
This project is configured with automated build management optimized for modern Linux/Ubuntu development environments.

### Build Instructions
```bash
# Compile with maximum production optimizations (-O3)
make
# Execute the simulation engine telemetry tests
./cache_sim

#Clean build artifacts
make clean
```

# Telemetry Tests
==============================================
  Initializing High-Performance C++ Page Cache Simulator
==============================================

-> Cache Miss for Page ID: 101. Fetching resource...
[Allocated] Page Frame 101 initialized in heap.
-> Cache Miss for Page ID: 102. Fetching resource...
[Allocated] Page Frame 102 initialized in heap.
-> Cache HIT for Page ID: 101
-> Cache Miss for Page ID: 103. Fetching resource...
 [Eviction Triggered] Removing Least Recently Used Page ID:102
[Deallocated] Page Frame 102 safely freed from memory.
[Allocated] Page Frame 103 initialized in heap.

==============================================
 Shutting down simulator (Cleaning up memory scopes)
==============================================
[Deallocated] Page Frame 103 safely freed from memory.
[Deallocated] Page Frame 101 safely freed from memory.



# High-Performance Userspace Page Cache Simulator (Modern C++)

A high-performance, deterministic userspace simulation of an Operating System Page Cache utilizing a Least Recently Used (LRU) eviction architeccture. This project models low-level virtual memory management constraints within a strictly bounded resource space, executing all core operations in deterministic  $O(1)$ constant time.

## Architectural Design & Time Complexity
To avoid the linear overhead ($O(1)$) of searching through sequential arrays, this engine couples two complementary data structures to decouple lookup latency from cache capacity:

1. **Recency Tracking Queue ('std::list'):** A doubly linked list acting as the recency tracking mechanics. Most Recently Used (MRU) nodes are kept at the head, while Least Recently Used (LRU) nodes reside at the tail.
2. **Instant Lookup Engine ('std::unordered_map'):** A hash table pairing unique age IDs directly to their respective iterator locations within the tracking queue, guaranteering $O(1)$ search mechanics.

### Hardware & OS Primitives Implemented:
* **Resource Acquistion Is Initialization (RAII):** Completely eliminates manual allocation risks ('malloc`/`free') by leveraging modern smart pointers ('std::unique_ptr').Heap allocations are strictly bound to container lifecycles.
* **Zero-Copy Data Transfer:** Utilizes C++ move semantics ('std::move') to transfer data payloads across interface boundaries, matching the zero-copy design principles of performance-critical network stacks.
* **Deterministic Eviction:** Capacity breaches instantly isolate and reclaim the oldest memory frames at the tail of the layout queue without latency spikes.

## Toolchain & Compilation
This project is configured with automated build management optimized for modern Linux/Ubuntu development environments.

### Build Instructions
```bash
# Compile with maximun production optimizations (-O3)
make
# Execute the simulation engine telemetry tests
./cache_sim

#Clean build artifacts
make clean


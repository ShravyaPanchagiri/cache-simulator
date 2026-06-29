#include <iostream>
#include <unordered_map>
#include <list>
#include <memory>
#include <string>
#include <cstdint>
#include <stdexcept>

// Represents a mock memory page frame managed by the OS architecture
class PageFrame {
public:
	uint32_t pageId;
	std::string data;

	PageFrame(uint32_t id, std::string payload)
		: pageId(id), data(std::move(payload)) {
			std::cout << "[Allocated] Page Frame " << pageId << " initialized in heap.\n";	
		}

		~PageFrame() {
			std::cout << "[Deallocated] Page Frame " << pageId << " safely freed from memory.\n";
		}
};

//High-performance User-space Page Cache Manager Engine

class PageCacheManager {
private:
	size_t maxCapacity;

	// Explicitly tracks ownership of page objects via smart pointers
	using PagePtr = std::unique_ptr<PageFrame>;

	// Doubly-linked list to track structural recency (Front = MRU, Back = LRU)
	std::list<uint32_t> lrulist;

	// O(1) Lookup engine pairing Page ID to its location in the list and its underlying data block
	std::unordered_map<uint32_t, std::pair<std::list<uint32_t>::iterator, PagePtr>> cacheMap;

public:
	explicit PageCacheManager(size_t capacity) : maxCapacity(capacity) {
		if(capacity == 0) throw std::invalid_argument("capacity must be >= 1");
	}

	// Simulates reading or writing a target page block from a hardware register interface
	void accessPage(uint32_t pageId, const std::string& fallbackPayload) {
		auto it = cacheMap.find(pageId);

		// Cache Hit path execution
		if(it != cacheMap.end()) {
			std::cout << " -> Cache HIT for Page ID: " << pageId << "\n";
			//lrulist.erase(it->second.first);
			//lrulist.push_front(pageId);
			lrulist.splice(lrulist.begin(), lrulist,it->second.first);
			it->second.first = lrulist.begin();
			return;
		}

		// Cache Miss path execution
		std::cout << "-> Cache Miss for Page ID: " << pageId << ". Fetching resource...\n";


		//Eviction handling if maximum capacity boundary is crossed
		if(cacheMap.size() >= maxCapacity){
			uint32_t oldestPageId = lrulist.back();
			std::cout << " [Eviction Triggered] Removing Least Recently Used Page ID:" << oldestPageId << "\n";

			lrulist.pop_back();
			cacheMap.erase(oldestPageId); // unique_ptr automatically frees the memory here safely
		}

		// Insert new object cleanly into the tracking frameworks
		lrulist.push_front(pageId);
		auto newPage = std::make_unique<PageFrame>(pageId, fallbackPayload);
		cacheMap[pageId] = { lrulist.begin(), std::move(newPage)};
	}
};

int main()
{
	std::cout << "===============================================================\n";
	std::cout << "  Initializing High-Performance C++ Page Cache Simulator\n";
	std::cout << "===============================================================\n\n";


	// Create a page cache that can only hold a maximum of 2 pages concurrently
	PageCacheManager systemCache(2);

	systemCache.accessPage(101, "Kernel_Init_Data");
	systemCache.accessPage(102, "Interrupt_Vector_Table");

	// This will cause a cache hit and refresh  Page 101's placement index ranking
	systemCache.accessPage(101, "Kernel_Init_Data");

	// This will exceed capacity boundaries and safely evict Page 102
	systemCache.accessPage(103, "PCIe_Subsystem_Config");

	std::cout << "\n==============================================================\n";
	std::cout << " Shutting down simulator (Cleaning up memory scopes)\n";
	std::cout << "=================================================================\n";
	return 0;
}
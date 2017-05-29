/*
 * Author: Carlos Eduardo Cirilo
 * Date: 28/05/2017
 * Purpose: This code implements a proposal for a Least Recently Used (LRU) cache.
*/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>

using namespace std;

/* A custom data structure representing a single page in memory (in this
 * implementation, memory is constituted by a set of MemoryPage nodes
 * organized as a Doubly Linked List).
*/
typedef struct MemoryPage {
	MemoryPage 	*prev, // a pointer to the previous memory page
				*next; // a pointer to the next node memory page
	string data; // the actual data which the memory page carries in
	unsigned key; //the key that unequivocally identifies the data carried in the memory page
} MemoryPage;

/* A custom data structure representing the memory as a whole (in this
 * implementation, memory is structured as a First In First Out (FIFO) queue).
*/
typedef struct Memory {
	MemoryPage 	*head, // a pointer to the page at first position in memory
				*tail; // a pointer to the page at last position in memory
} Memory;

// A helper custom type to store cache's index as a associative key-value container
typedef map<unsigned,MemoryPage*> CacheIndex;

/* A custom data structure representing the cache abstraction for this program's purposes
 * (in this implementation, cache is synthetized as a structure that keeps a reference to the
 * memory area occupied by the cache, along with a map container indexing the entries in cache).
*/
typedef struct Cache {
	unsigned capacity; // how many memory pages cache is allowed to have
	Memory memory;  //a pointer to the memory area used by the cache
	CacheIndex index; // an array of pointers to the memory pages in cache
} Cache;


/*
 * The LRUCache class encapsulates the routines and data related to
 * cache maniputalion, furnishing the basic opearations: (i) to build a LRU cache so as
 * to make it available to receive new entries and to be queried for some entry;
 * (ii) to insert entries in cache; (iii) to read the value of a given entry in cache
 * given its key; (iv) and, finally, to purge the cache from memory, by destructing it
 * and freeing the memory it was using.
*/
class LRUCache {
	private:
		Cache cache;

        // A helper function to check if cache has reached its full capacity
		bool isCacheFull() {
            return cache.index.size() == cache.capacity;
        };

        // A helper function to check if cache has no page in it
		bool isCacheEmpty() {
            return cache.index.size() == 0;
		};

		// A helper function to instantiate a new MemoryPage node
		MemoryPage* newMemoryPage(unsigned key, string data) {
            // Allocates a new page into host's memory and assigns properties 'data' and 'key' to it
            MemoryPage *newPage = new MemoryPage;
            newPage->data = data;
            newPage->key = key;

            // Initializes 'prev' and 'next' pointers as NULL
            newPage->prev = newPage->next = NULL;

            return newPage;
        };

        // A helper function to get more room on cache's memory by deleting the page at last position in cache's memory
		void makeARoom() {
            if(isCacheEmpty() || !isCacheFull()) // Imediately returns from this function if it has been called in a context in which cache is either empty or not full
                return;

            if (cache.memory.head == cache.memory.tail) // If cache's memory has just one page in it, updates the 'memory.head' pointer to NULL
                cache.memory.head = NULL;

            // Makes 'memory.tail' pointer to point to the memory page preceeding the one being removed
            MemoryPage *pageToBeRemoved = cache.memory.tail;
            cache.memory.tail = pageToBeRemoved->prev;
            if (cache.memory.tail != NULL)
                cache.memory.tail->next = NULL;

            cout << "Cache is full. Removing key " << pageToBeRemoved->key << " from cache before inserint a new one..." << endl;

            // Releases the cache's index slot referring to the page being removed,
            // and frees the space in memory that was occupied by it
            cache.index.erase(pageToBeRemoved->key);
            delete pageToBeRemoved;
        };

        void dump() {
            cout << "Cache's memory state: " << "[  ";

            MemoryPage *memoryPage = cache.memory.head;

            while (memoryPage != NULL) {
                cout << memoryPage->key << " => " << memoryPage->data << ",  ";
                memoryPage = memoryPage->next;
            }

            cout << "]" << endl;
		};

		void dumpIndex() {
            cout << "Cache's index state: " << "[  ";


            for (map<unsigned,MemoryPage*>::iterator indexEntry = cache.index.begin(); indexEntry != cache.index.end(); ++indexEntry)
                cout << indexEntry->first << " => " << indexEntry->second << ",  ";

            cout << "]" << endl;
        };
	public:
        // Calss' constructor.
		LRUCache() {
            build(0); // Invokes build() function with argument 0 so as to safely initialize cache's fundamental properties by the time of the class instantiation
		};

        // Creates a cache with N possible entries
		void build(unsigned n) {
            if (n > 0)
                cout << "Building a LRU Cache with capacity for " << n << " entries..." << endl;
            else
                cout << "Setting cache into a new empty state..." << endl;

            cache.capacity = n;

            for (map<unsigned,MemoryPage*>::iterator indexEntry = cache.index.begin(); indexEntry != cache.index.end(); ++indexEntry) {
                cache.index.erase(indexEntry->first);
                delete (indexEntry->second);
            }

            cache.memory.head = cache.memory.tail = NULL;
            dump();
            dumpIndex();
        };

        // Cleans all cache entries and any other data related to the cache itself
		void destruct() {
            cout << "Destructing LRU Cache along with all data related to it..." << endl;
            build(0);
		};

        // Adds a new entry into cache's memory and updates the cache's index. If cache is out of its capacity,
        // this function firstly removes the least referred page from cache's before inserting the new entry
		void add(unsigned key, string data) {
            cout << "Adding data to key: " << key << endl;

            map<unsigned,MemoryPage*>::iterator indexEntry = cache.index.find(key);

            if (indexEntry != cache.index.end()) {  // If cache has already an entry for the given key, then purges it from cache's memory first
                cout << "Key " << key << " has already an entry in the cache. Replacing it..." << endl;

                //cache.index.erase(key);

                bool isHead = false;
                bool isTail = false;

                if (cache.memory.head == indexEntry->second) {
                    isHead = true;

                    cache.memory.head = indexEntry->second->next;

                    if (cache.memory.head != NULL) {
                        cache.memory.head->prev = NULL;
                    }
                }

                if (cache.memory.tail == indexEntry->second) {
                    isTail = true;
                    cache.memory.tail = indexEntry->second->prev;

                    if (cache.memory.tail != NULL) {
                        cache.memory.tail->next = NULL;
                    }
                }

                if (!isHead && !isTail) {
                    indexEntry->second->prev->next = indexEntry->second->next;
                    indexEntry->second->next->prev = indexEntry->second->prev;
                }

                delete (indexEntry->second);
            } else if (isCacheFull()) { // If cache has not the entry fro the given key yet, and it is full, then makes a room by removing the page at last position in cache's memory
                makeARoom();
            }

            // Creates a new memory page node with given key and data,
            // and adds it to the first position in cache's memory
            MemoryPage *newPage = newMemoryPage(key, data);
            newPage->next = cache.memory.head;

            if (isCacheEmpty()) {  // If queue is empty, makes both pointers 'memory.head' and 'memory.tail' to point to the address of the newly created page
                cache.memory.head = cache.memory.tail = newPage;
            } else { // Otherwise, just makes 'memory.head' pointing to the new page and updates the property 'prev' of the page that was at memory head
                cache.memory.head->prev = newPage;
                cache.memory.head = newPage;
            }

            // Inserts the new page entry into cache's index container
            cache.index[key] = newPage;
            dump();
            dumpIndex();
        };

        // Queries the cache for data identified by the given key
        // If there is no data in cache for the key informed, throwns an exception.
        // Otherwise, returns the data and moves the page carrying it to the first position at cache's memory
		string get(unsigned key) {
            cout << "Reading data from key: " << key << endl;

            map<unsigned,MemoryPage*>::iterator indexEntry = cache.index.find(key);

            if (indexEntry != cache.index.end()) {
                MemoryPage *foundMemoryPage = indexEntry->second;
                if (foundMemoryPage != cache.memory.head) { // The data for the key informed is there, but the page carrying it is not at the first position of cache's memory. So, updates it
                    // Unties the requested page from its current location in cache's memory.
                    foundMemoryPage->prev->next = foundMemoryPage->next;
                    if (foundMemoryPage->next)
                       foundMemoryPage->next->prev = foundMemoryPage->prev;

                    // If the requested page is at last position in cache's memory, then updates 'memory.tail' pointer as long as it is going to be moved to the first position now
                    if (foundMemoryPage == cache.memory.tail) {
                       cache.memory.tail = foundMemoryPage->prev;
                       cache.memory.tail->next = NULL;
                    }

                    // Places the requested page before current first page in cache's memory
                    foundMemoryPage->next = cache.memory.head;
                    foundMemoryPage->prev = NULL;

                    // Makes 'prev' pointer of former first page pointing to the new first page
                    foundMemoryPage->next->prev = foundMemoryPage;

                    // Updates 'memory.head' pointer to the requested page's address
                    cache.memory.head = foundMemoryPage;

                    dump();
                    dumpIndex();
                }
                return foundMemoryPage->data;
            } else {
                cerr << "Cache page fault: Key " << key << " does not exist in cache!" << endl;
                throw std::invalid_argument("Unknown key!");
            }
        };
};


int main(int argc, char** argv) {
	const unsigned N =4;
	LRUCache lruCache;

	lruCache.build(N);

	lruCache.add(0, "Marvin");
	lruCache.add(1, "Ford Prefect");
	lruCache.add(0, "Another Marvin");
	lruCache.add(10, "Lisbeth Salander");
	lruCache.add(3, "Mikael Blomkvist");
	lruCache.add(4, "Trician McMillian");
	lruCache.add(5, "Don't panic!");

	try {
        lruCache.get(3);
        lruCache.get(11);
	} catch (const std::invalid_argument& e) {
        cerr << "An error has ocurred while reading data from cache: " << e.what() <<  endl;
	}

    lruCache.destruct();

    try {
        lruCache.get(5);
    } catch (const std::invalid_argument& e) {
        cerr << "An error has ocurred while reading data from cache: " << e.what() <<  endl;
	}


	lruCache.build(2);
	lruCache.add(0, "Marvin");
	lruCache.add(1, "Ford Prefect");
	lruCache.add(3, "Mikael Blomkvist");
	lruCache.destruct();

	return 0;
}

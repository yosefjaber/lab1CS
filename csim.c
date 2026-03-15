/*
 *
 * Name: Yosef Jaber
 * UC ID: [Nice Try]
 *
 */

#include "lab1.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

/*
 * CacheLine - Represents one line in a cache set
 *
 * valid: 1 if line contains data, 0 if empty
 * tag: Identifies which memory block is stored here
 * LRU_val: Timestamp of last access, used for LRU eviction
 */
typedef struct {
    int valid;  
    int tag; 
    int LRU_val;
} CacheLine;

/*
 * CacheSet - Represents one set in a cache
 *
 * lines: Array of CacheLines that are in the set 
 */
typedef struct {
    CacheLine *lines; 
} CacheSet;

/*
 * Cache - Represents a memory Cache
 *
 * sets: Array of Sets in the cache 
 */
typedef struct {
    CacheSet *sets;  
} Cache;

/* Global cache parameters and counters - shared across functions */
int s, E, b, hits, misses, evictions; 

/*
 * least_used_line_index - Finds the least recently used line in a set
 *
 * set: Pointer to the cache set to search
 *
 * Returns the index of the LRU line so it can be evicted
 */
int least_used_line_index(CacheSet *set) {
    int least_used_line_index = 0;

    // Look through all of the lines in the set and compare to the current least used line index updating when necessary 
    for(int i = 0; i < E; i++) {
        // If a completely unused line is found return it
        if (set->lines[i].valid == 0) {
            return i;
        }

        if((set->lines[i].LRU_val < set->lines[least_used_line_index].LRU_val)) {
            least_used_line_index = i;
        }
    }
    return least_used_line_index;
}

/*
 * access_memory - Simulates one memory access on the cache
 *
 * address: Memory address being accessed
 * cache:   Pointer to the cache to simulate
 *
 * Updates global hits, misses, and evictions accordingly
 */
void access_memory(unsigned long address, Cache *cache, const int counter) {
    // Use bit masking to isolate each section of the address
    int set_index = (address >> b) & ((1 << s) - 1);
    int tag = (address >> (b+s));

    CacheSet *set = &cache->sets[set_index]; // Set belonging to the address

    // Check each line in the set and check the tag for the address
    for (int i = 0; i < E; i++) {
        if (set->lines[i].valid == 1 && set->lines[i].tag == tag) {
            set->lines[i].LRU_val = counter;
            hits++;
            return;
        }
    }

    // Find the least used line index to evict
    int eject_block_index = least_used_line_index(set);

    // If it is an invalid block then it is just a miss otherwise it is a miss AND an eviction
    if (set->lines[eject_block_index].valid == 0) {
        misses++;
    }
    else {
        misses++;
        evictions++;
    }

    // Create new line and put it in the eject block index
    CacheLine new_line;
    new_line.valid = 1;
    new_line.tag = tag;
    new_line.LRU_val = counter;
    set->lines[eject_block_index] = new_line;
}

/*
 * main - Entry point for the cache simulator
 *
 * argc: Number of command line arguments
 * argv: Array of command line argument strings
 *
 * Parses arguments, builds cache, reads trace file,
 * simulates accesses, and prints final hit/miss/eviction summary
 */
int main(int argc, char *argv[]) {
    int opt, counter, size;
    counter = 0;

    char op;
    unsigned long address;
    char *trace_file;

    // Read flags
    while ((opt = getopt(argc, argv, "s:E:b:t:")) != -1) {
        switch (opt) {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
        }
    }

    // Make the cache and fill it up with default values
    Cache cache;
    cache.sets = malloc((1 << s) * sizeof(CacheSet));

    for (int i = 0; i < (1 << s); i++) {
        cache.sets[i].lines = malloc(E * sizeof(CacheLine));
        for (int j = 0; j < E; j++) {
            cache.sets[i].lines[j].valid = 0;
            cache.sets[i].lines[j].tag = 0;
            cache.sets[i].lines[j].LRU_val = 0;
        }
    }


    // Read the file and access memory one if it is a load or save, access memory twice if it is a modify
    FILE *file = fopen(trace_file, "r"); 

    while ((fscanf(file, " %c %lx,%d", &op, &address, &size) == 3)) {
        if(op == 'I') {
            continue;
        }

        // Load or Store
        if(op == 'L' || op == 'S') {
            access_memory(address, &cache, counter);
        }

        // Load and Store
        if(op == 'M') {
            access_memory(address, &cache, counter);
            access_memory(address, &cache, counter);
        }
        counter++;
    }
    fclose(file);

    printSummary(hits, misses, evictions);

    // Free memory
    for (int i = 0; i < (1<<s); i++) {
        free(cache.sets[i].lines);
    }
    free(cache.sets);

    return 0;
}

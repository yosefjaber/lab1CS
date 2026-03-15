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

int s, E, b, byte_offset, set_index, tag, counter, hits, misses, evictions, size; 
char op;
unsigned long address;

/*
 * least_used_line_index - Finds the least recently used line in a set
 *
 * set: Pointer to the cache set to search
 *
 * Returns the index of the LRU line so it can be evicted
 */
int least_used_line_index(CacheSet *set) {
    int least_used_line_index = 0;
    for(int i = 1; i < E; i++) {
        if((set->lines[i].LRU_val < set->lines[least_used_line_index].LRU_val) || (set->lines[i].tag == 0 && set->lines[least_used_line_index].LRU_val == 1)) {
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
void access_memory(unsigned long address, Cache *cache) {
    byte_offset = address & ((1 << b) - 1);
    set_index = (address >> b) & ((1 << s) - 1);
    tag = (address >> (b+s));

    CacheSet *set = &cache->sets[set_index];

    for (int i = 0; i < E; i++) {
        if (set->lines[i].valid == 1 && set->lines[i].tag == tag) {
            set->lines[i].LRU_val = counter;
            hits++;
            return;
        }
    }

    int eject_block_index = least_used_line_index(set);

    if(set->lines[eject_block_index].valid == 0) {
        misses++;
    }
    else {
        misses++;
        evictions++;
    }

    CacheLine new_line;
    new_line.valid = 1;
    new_line.tag = tag;
    new_line.LRU_val = counter;
    set->lines[eject_block_index] = new_line;
}

int main(int argc, char *argv[]) {
    int opt;
    char *trace_file;

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

    Cache cache;
    cache.sets = malloc((1 << s) * sizeof(CacheSet));

    for (int i = 0; i < (1 << s); i++) {
        cache.sets[i].lines = malloc(E * sizeof(CacheLine));
        for (int j = 0; j < E; j++) {
            cache.sets[i].lines[j].valid = 0;
            cache.sets[i].lines[j].tag = 0;
        }
    }

    FILE *file = fopen(trace_file, "r"); 

    while ((fscanf(file, " %c %lx,%d", &op, &address, &size) == 3)) {
        if(op == 'I') {
            continue;
        }

        // Load or Store
        if(op == 'L' || op == 'S') {
            access_memory(address, &cache);
        }

        // Load and Store
        if(op == 'M') {
            access_memory(address, &cache);
            access_memory(address, &cache);
        }
        counter++;
    }

    printSummary(hits, misses, evictions);

    // free memory
    for (int i = 0; i < (1<<s); i++) {
        free(cache.sets[i].lines);
    }
    free(cache.sets);

    return 0;
}

#include "lab1.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

typedef struct {
    int valid;  
    int tag; 
    int LRU_val;
} CacheLine;

typedef struct {
    CacheLine *lines; 
} CacheSet;

typedef struct {
    CacheSet *sets;  
} Cache;

int s, E, b, byte_offset, set_index, tag, counter, hits, misses, evictions, size; 
char op;
unsigned long address;

int least_used_line_index(CacheSet *set) {
    int least_used_line_index = 0;
    for(int i = 1; i < E; i++) {
        if((set->lines[i].LRU_val < set->lines[least_used_line_index].LRU_val) || (set->lines[i].tag == 0 && set->lines[least_used_line_index].LRU_val == 1)) {
            least_used_line_index = i;
        }
    }
    return least_used_line_index;
}

void access_memory(int address, Cache *cache) {
    byte_offset = address & ((1 << b) - 1);
    set_index = (address >> b) & ((1 << s) - 1);
    tag = (address >> (b+s));

    CacheSet *set = &cache->sets[set_index];

    for (int i = 0; i < E; i++) {
        //hit
        if (set->lines[i].tag == tag) {
            set->lines[i].LRU_val = counter;
            hits++;
        }
    }

    //miss
    int eject_block_index = least_used_line_index(set);

    if(set->lines[eject_block_index].tag == 0) {
        misses++;
    }
    else {
        misses++;
        evictions++;
    }

    //Do i have to free memory here before I do this?
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

    int hits = 0;
    int misses = 0;

    while ((fscanf(file, " %c %lx, %d", &op, &address, &size) == 3)) {
        if(op == 'I') {
            continue;
        }

        // Load or Store
        if(op == 'L' || op == 'S') {
            for (int i = 0; i < size; i++) {
                access_memory(address+i, &cache);
            }
        }

        // Load and Store
        if(op == 'M') {
            for (int i = 0; i < size; i++) {
                access_memory(address+i, &cache);
                access_memory(address+i, &cache);
            }
        }
        counter++;
    }


    printSummary(hits, misses, evictions);
    return 0;
}

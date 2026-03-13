#include "lab1.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int valid;  
    int tag; 
    int LRU_val;
} CacheLine;

typedef struct {
    CacheLine[] *lines; 
} CacheSet;

typedef struct {
    CacheSet[] *sets;  
} Cache;

int least_used_line_index(CacheSet *set) {
    int least_used_line_index = 0;
    for(int i = 1; i < E, i++) {
        if(set.lines[i].LRU_val < least_used_line_index) {
            least_used_line_index = i;
        }
    }
    return least_used_line_index;
}

bool access_memory(int address, Cache *MainMemory) {
    Result results;

    byte_offset = address & ((b << 1) - 1);
    set_index = (address >> b) & ((s << 1) - 1);
    tag = (address >> (b+s));

    CacheSet *set = Cache.sets[set_index];

    for (int i = 0; i < E, i++) {
        //hit
        if (set.lines[i].tag == tag) {
            results.hits++;
            set.lines[i].LRU_value = counter;
            return true;
        }
    }

    //miss
    int eject_block_index = least_used_line_index(set);

    //Do i have to free memory here before I do this?
    CacheLine new_line;
    new_line.valid = 1;
    new_line.tag = tag;
    new_line.LRU_val = counter;
    set.lines[eject_block_index] = new_line;
}

int s, E, b, byte_offset, set_index, tag, counter; 

int main(int argc, char *argv[])
{
    //s is how many sets there are
    //E is how many lines in each set there are
    //b is how many bytes are in each line
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

    Cache MainMemory;

    for(int i = 0; i < s; i++)
    {
        CacheSet set;
        for(int j = 0; j < E; j++)
        {
            CacheLine line;
            line.valid = 0x0;
            line.tag = 0x0;
            set[j] = line;
        }
        MainMemory[i] = set;
    }

    FILE *trace_file = fopen(trace_file, "r"); 

    int hits = 0;
    int misses = 0;

    while (fscanf(trace_file, " %c %lx, %d", &op, &address, &size) = 3) {
        if(op == "I") {
            continue;
        }

        // Load or Store
        if(op == "L" || op == "S") {
            for (int i = 0; i < size; i++) {
                if (access_memory(address+i)) {
                    hits++;
                }
                else {
                    misses++;
                }
            }
        }

        // Load and Store
        if(op == "M") {
            for (int i = 0; i < size; i++) {
                if (access_memory(address+i)) {
                    hits++;
                }
                else {
                    misses++;
                }

                if (access_memory(address+i)) {
                    hits++;
                }
                else {
                    misses++;
                }
            }
        }
        counter++;
    }


    printSummary(0, 0, 0);
    return 0;
}

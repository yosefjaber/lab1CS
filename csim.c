#include "lab1.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

typedef struct {
    int valid;  
    int tag; 
} CacheLine;

typedef struct {
    CacheLine *lines; 
} CacheSet;

typedef struct {
    CacheSet *sets;  
} Cache;

int main(int argc, char *argv[])
{
    int s, E, b, opt;
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
            line.valid, line.tag = 0x0;
        }
    }




    //printSummary(0, 0, 0);
    return 0;
}

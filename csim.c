#include "lab1.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("%s\n %s\n",argv[0], argv[1]);

    char *sets = argv[1];
    char *lines_per_set = argv[2];
    char *bytes_per_block = argv[3];
    char *trace_file = argv[4];

    printf("\n%s", trace_file);


    //printSummary(0, 0, 0);
    return 0;
}

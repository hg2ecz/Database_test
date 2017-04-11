#include <stdio.h>
#include <time.h>
#include "search.h"

#define REPEAT 10

int main(int argc, char **argv) {
    if (argc != 2) {
	fprintf(stderr, "Paraméter: keresendő szám.\n");
	return -1;
    }
    char *value = argv[1];

    const struct _func {
	void (*fname)(char *value);
	char name[40];
    } func[] = {
	{search_fopen, "search_fopen"},
	{search_mmap, "search_mmap"},
	{search_indexuse_unordered, "search_indexuse_unordered"},
	{search_indexuse_partially, "search_indexuse_partially"},
	{search_indexuse_partially_multi, "search_indexuse_partially_multi"},
	{search_indexuse_ordered, "search_indexuse_ordered"},
	{NULL, "\0"}
    };

    struct timespec tstart, tend;
    for (int f=0; func[f].fname; f++) {
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tstart);
	for (int i=0; i<REPEAT; i++) func[f].fname(value);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tend);
	double eltime_ms = 1000.0*(tend.tv_sec - tstart.tv_sec) + (tend.tv_nsec - tstart.tv_nsec)/1000000.;
	printf("%s ==> %9.5f ms/db\n\n", func[f].name, eltime_ms/REPEAT);
    }
    return 0;
}

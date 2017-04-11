#include <stdio.h>
#include <time.h>
#include "search.h"

#define REPEAT 10

int tcalc(struct timespec t[2], const char *melyik) {
    double eltime=1000.0*(t[1].tv_sec - t[0].tv_sec) + (t[1].tv_nsec - t[0].tv_nsec)/1000000.;
    printf("%s ==> %9.5f ms/db\n\n", melyik, eltime/REPEAT);
    return eltime;
}

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

    struct timespec t[2];
    for (int f=0; func[f].fname; f++) {
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[0]);
	for (int i=0; i<REPEAT; i++) func[f].fname(value);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[1]); tcalc(t, func[f].name);
    }
    return 0;
}

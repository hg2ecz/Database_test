#include <stdio.h>
#include <time.h>
#include "search.h"

#define REPEAT 10
int tcalc(struct timespec t[2], char *melyik) {
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

    struct timespec t[2];
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[0]);
    for (int i=0; i<REPEAT; i++) search_fopen(value);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[1]); tcalc(t, "search_fopen");

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[0]);
    for (int i=0; i<REPEAT; i++) search_mmap(value);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[1]); tcalc(t, "search_mmap");

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[0]);
    for (int i=0; i<REPEAT; i++) search_indexuse_unordered(value);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[1]); tcalc(t, "search_unordered");

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[0]);
    for (int i=0; i<REPEAT; i++) search_indexuse_ordered(value);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[1]); tcalc(t, "search_ordered");
    return 0;
}

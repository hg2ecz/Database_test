#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "search.h"

#define CHUNKSIZE (1<<LOG2CHUNKSIZE)

void search_indexuse_partially_multi(char *value) {
    int valint = atoi(value);

    struct stat statbuf;
    int fd = open("dblogfile-idx_partially_multi.log", O_RDONLY);
    if (fd < 0) {
	fprintf(stderr, "dblogfile-idx_partially_multi.log not exist, please run indexgen_unordered_ordered.\n");
	return;
    }
    fstat(fd, &statbuf);

    struct _idx {
	int num;
	int pos;
    } *idx = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED,fd, 0);

    int fd_log = open("dblogfile.log", O_RDONLY);
    if (fd_log < 0) {
	fprintf(stderr, "dblogfile.log not exist, please run generate_dbfile.py.\n");
	return;
    }
    int maxnum = statbuf.st_size/sizeof(struct _idx);

// Here is indexnum/CHUNKSIZE pieces of ordered part
    int maxchunk=CHUNKSIZE;
    int idxremain=maxnum;
    while (maxchunk <= idxremain) maxchunk<<=1; // double size
    while (idxremain >= CHUNKSIZE) {
	maxchunk >>= 1;

	int diff = maxchunk>>1;
	int ptr = (maxnum-idxremain) + diff;
	while (diff) {
	    diff >>= 1;
	    ptr += idx[ptr].num < valint ? diff : -diff;
	}
	if (idx[ptr].num < valint) ptr++;
	idxremain -= maxchunk; // new remain

	char buf[1024];
	int rowlen;

	while (idx[ptr].num == valint && ptr < maxnum-idxremain) { // new remain!
	    lseek(fd_log, idx[ptr].pos, SEEK_SET);
	    if (read(fd_log, buf, sizeof(buf)) < 0) puts("read error\n");
	    for (rowlen=0; buf[rowlen]!='\n' && rowlen < sizeof(buf); rowlen++);
	    if (write(1, buf, ++rowlen) < 0) puts("write error\n");
	    ptr++;
	}

    }

// Last: unordered part
    for (int i=maxnum-idxremain; i<maxnum; i++) {
	if (idx[i].num == valint) {
	    char buf[1000];
	    lseek(fd_log, idx[i].pos, SEEK_SET);
	    if (read(fd_log, buf, idx[i+1].pos-idx[i].pos) < 0) puts("read error\n");
	    if (write(1, buf, idx[i+1].pos-idx[i].pos) < 0) puts("write error\n");
	}
    }

    close(fd_log);
    close(fd);
}


#ifdef SELFTEST
int main(int argc, char **argv) {
    search_indexuse_partially_multi(argv[1]);
}
#endif

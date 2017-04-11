#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "search.h"

#define CHUNKSIZE (1<<LOG2CHUNKSIZE)

void search_indexuse_partially(char *value) {
    int valint = atoi(value);

    struct stat statbuf;
    int fd = open("dblogfile-idx_partially.log", O_RDONLY);
    if (fd < 0) {
	fprintf(stderr, "dblogfile-idx_partially.log not exist, please run indexgen_unordered_ordered.\n");
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
    int pn;
    for (pn=0; pn<maxnum/CHUNKSIZE; pn++) {
	int diff = CHUNKSIZE>>1;
	int ptr = pn*CHUNKSIZE + diff;
	while (diff) {
	    diff >>= 1;
	    ptr += idx[ptr].num < valint ? diff : -diff;
	}
	if (idx[ptr].num < valint) ptr++;

	char buf[1024];
	int rowlen;

	while (idx[ptr].num == valint && ptr < (pn+1)*CHUNKSIZE) {
	    lseek(fd_log, idx[ptr].pos, SEEK_SET);
	    read(fd_log, buf, sizeof(buf));
	    for (rowlen=0; buf[rowlen]!='\n' && rowlen < sizeof(buf); rowlen++);
	    write(1, buf, ++rowlen);
	    ptr++;
	}
    }

// Last: unordered part
    for (int i=pn*CHUNKSIZE; i<maxnum; i++) {
	if (idx[i].num == valint) {
	    char buf[1000];
	    lseek(fd_log, idx[i].pos, SEEK_SET);
	    read(fd_log, buf, idx[i+1].pos-idx[i].pos);
	    write(1, buf, idx[i+1].pos-idx[i].pos);
	}
    }

    close(fd_log);
    close(fd);
}


#ifdef SELFTEST
int main(int argc, char **argv) {
    search_indexuse_partially(argv[1]);
}
#endif

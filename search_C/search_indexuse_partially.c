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

    struct stat statbuf, logfilestatbuf;
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
    fstat(fd_log, &logfilestatbuf);
    int maxidx = statbuf.st_size/sizeof(struct _idx);

// Here is indexnum/CHUNKSIZE pieces of ordered part
    int pn;
    for (pn=0; pn<maxidx/CHUNKSIZE; pn++) {
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
	    if (read(fd_log, buf, sizeof(buf)) < 0) puts("read error1\n");
	    for (rowlen=0; buf[rowlen]!='\n' && rowlen < sizeof(buf); rowlen++);
	    if (write(1, buf, ++rowlen) < 0) puts("write error1\n");
	    ptr++;
	}
    }

// Last: unordered part
    for (int i=pn*CHUNKSIZE; i<maxidx; i++) {
	if (idx[i].num == valint) {
	    int res;
	    char buf[1000];
	    lseek(fd_log, idx[i].pos, SEEK_SET);
	    if ((res = read(fd_log, buf, (i<maxidx-1 ? idx[i+1].pos : logfilestatbuf.st_size) - idx[i].pos)) < 0) puts("read error2\n");
	    if (write(1, buf, res) < 0) puts("write error2\n");
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

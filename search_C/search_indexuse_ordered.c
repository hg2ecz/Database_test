#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "search.h"

void search_indexuse_ordered(char *value) {
    int valint = atoi(value);

    struct stat statbuf;
    int fd = open("dblogfile-idx_ordered.log", O_RDONLY);
    if (fd < 0) {
	fprintf(stderr, "dblogfile-idx_ordered.log not exist, please run indexgen_unordered_ordered.\n");
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
    int diff = (maxnum+1)/2;
    int ptr = diff;
    while (diff > 1) {
	diff = (diff+1)/2;
	ptr += idx[ptr].num < valint ? diff : -diff;
    }
    if (idx[ptr].num < valint) ptr++;

    char buf[1024];
    int rowlen;

    while (idx[ptr].num == valint && ptr < maxnum) {
	lseek(fd_log, idx[ptr].pos, SEEK_SET);
	read(fd_log, buf, sizeof(buf));
	for (rowlen=0; buf[rowlen]!='\n' && rowlen < sizeof(buf); rowlen++);
	write(1, buf, ++rowlen);
	ptr++;
    }

    close(fd_log);
    close(fd);
}


#ifdef SELFTEST
int main(int argc, char **argv) {
    search_indexuse_ordered(argv[1]);
}
#endif

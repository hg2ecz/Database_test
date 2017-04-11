#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "search.h"

void search_indexuse_unordered(char *value) {
    int valint = atoi(value);

    struct stat statbuf;
    int fd = open("dblogfile-idx_unordered.log", O_RDONLY);
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
    for (int i=0; i<statbuf.st_size/sizeof(struct _idx); i++) {
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
    search_indexuse_unordered(argv[1]);
}
#endif

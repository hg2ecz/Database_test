#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define CHUNKSIZE (1<<LOG2CHUNKSIZE)

struct _idx {
    int num;
    int pos;
};

int compare(const void *ain, const void *bin) {
    struct _idx *a = (struct _idx *)ain;
    struct _idx *b = (struct _idx *)bin;

    if (a->num > b->num) return 1;
    else if (a->num < b->num) return -1;
    else return 0;
}

void indexgen() {
    struct stat statbuf;
    int fd = open("dblogfile.log", O_RDONLY);
    fstat(fd, &statbuf);
    char *p = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED,fd, 0);

    int idxmaxnum = 1024;
    int idxnum = 0;

    struct _idx *idx = malloc(idxmaxnum*sizeof(struct _idx));

    size_t start=0, ptr=0;
    while (ptr < statbuf.st_size) {
	for ( ; p[ptr] != '\n' && ptr < statbuf.st_size; ptr++);
	ptr++;

	if ( idxnum >= idxmaxnum) {
	    idxmaxnum *= 2;
	    idx = realloc(idx, idxmaxnum*sizeof(struct _idx));
	}

	int i;
	for (i=start; p[i]!=';'; i++);
	i++;
	int num = 0;
	for (; p[i]!=';'; i++) num = num*10 + p[i]-'0';
	idx[idxnum].num = num;
	idx[idxnum++].pos = start;
	start = ptr;
    }
    close(fd);

    // unordered
    int fd_idx;
    fd_idx = open("dblogfile-idx_unordered.log", O_WRONLY | O_CREAT, 0644);
    if (write(fd_idx, idx, idxnum*sizeof(struct _idx)) < 0) puts("write error\n");
    close(fd_idx);

    // CHUNKSIZE + last
    for (int i=0; i<idxnum/CHUNKSIZE; i++) {
	qsort(&idx[i*CHUNKSIZE], CHUNKSIZE, sizeof(struct _idx), compare);
    }
    fd_idx = open("dblogfile-idx_partially.log", O_WRONLY | O_CREAT, 0644);
    if (write(fd_idx, idx, idxnum*sizeof(struct _idx)) < 0) puts("write error\n");
    close(fd_idx);

    // more (2^N)*CHUNKSIZE ... + last
    int maxchunk=CHUNKSIZE;
    int idxremain=idxnum;
    while (maxchunk <= idxremain) maxchunk<<=1; // double size
    while (idxremain >= CHUNKSIZE) {
	maxchunk >>= 1;
	qsort(&idx[idxnum-idxremain], maxchunk, sizeof(struct _idx), compare);
	idxremain -= maxchunk;
    }
    fd_idx = open("dblogfile-idx_partially_multi.log", O_WRONLY | O_CREAT, 0644);
    if (write(fd_idx, idx, idxnum*sizeof(struct _idx)) < 0) puts("write error\n");
    close(fd_idx);

    // full ordered
    qsort(idx, idxnum, sizeof(struct _idx), compare);
    fd_idx = open("dblogfile-idx_ordered.log", O_WRONLY | O_CREAT, 0644);
    if (write(fd_idx, idx, idxnum*sizeof(struct _idx)) < 0) puts("write error\n");
    close(fd_idx);
}

int main() {
    indexgen();
}

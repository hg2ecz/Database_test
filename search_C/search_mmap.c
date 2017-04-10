#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "search.h"

void search_mmap(char *value) {
    struct stat statbuf;
    int fd = open("dblogfile.log", O_RDONLY);
    if (fd < 0) {
	fprintf(stderr, "dblogfile.log not exist, please run generate_dbfile.py.\n");
	return;
    }
    fstat(fd, &statbuf);
    char *p = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED,fd, 0);

    size_t start=0, ptr=0;
    while (ptr < statbuf.st_size) {
	for ( ; p[ptr] != '\n' && ptr < statbuf.st_size; ptr++);
	ptr++;

	int i, j;
	for (i=start; p[i]!=';'; i++);
	i++;
	for (j=0; p[i] == value[j]; i++, j++);
	if (p[i] == ';' && value[j] == '\0') write(1, &p[start], ptr-start);
	start = ptr;
    }
    close(fd);
}

#ifdef SELFTEST
int main() {
    search_mmap("14459606");
}
#endif

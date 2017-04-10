#include <stdio.h>
#include "search.h"

void search_fopen(char *value) {
    char row[100];
    FILE *f = fopen("dblogfile.log", "r");
    if (!f) {
	fprintf(stderr, "dblogfile.log not exist, please run generate_dbfile.py.\n");
	return;
    }
    while (fgets(row, sizeof(row), f)) {
	int i, j;
	for (i=0; row[i]!=';'; i++);
	i++;
	for (j=0; row[i] == value[j]; i++, j++);
	if (row[i] == ';' && value[j] == '\0') printf("%s", row);
    }
    fclose(f);
}

#ifdef SELFTEST
int main() {
    search_fopen("14459606");
}
#endif

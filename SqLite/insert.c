#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sqlite3.h>

void do_exit(sqlite3 *db, char *err_msg) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    exit(1);
}

int main() {
    sqlite3 *db;
//    sqlite3_stmt *res;
    char *err_msg=0;
    char query[1000];

    int rc = sqlite3_open("dblog.sqlite", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db, "drop table if exists test", 0, 0, &err_msg);
    if (rc != SQLITE_OK ) do_exit(db, err_msg);

    sprintf(query, "create table test (keyint int not null, nokeyint int not null, str varchar(50))");
    rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) do_exit(db, err_msg);

//    rc = sqlite3_exec(db, "alter table test add key(keyint)", 0, 0, &err_msg);
    rc = sqlite3_exec(db, "CREATE INDEX keyint_idx ON test (keyint)", 0, 0, &err_msg);
    if (rc != SQLITE_OK ) do_exit(db, err_msg);

    rc = sqlite3_exec(db, "BEGIN", 0, 0, &err_msg);
    if (rc != SQLITE_OK ) do_exit(db, err_msg);

    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    FILE *f = fopen("dblogfile.log", "r");
    char row[500];
    while (fgets(row, sizeof(row), f)) {
	char *field[3];
	int i, j;
	for (i=0, j=0; j<3; j++) {
	    field[j] = &row[i];
	    for (i=0; row[i]!=';' && row[i] != '\n'; i++);
	    row[i++] = '\0';
	}
	sprintf(query, "INSERT INTO test (keyint, nokeyint, str) values (%s, %s, '%s')", field[1], field[1], field[2]);

	rc = sqlite3_exec(db, query, 0, 0, &err_msg);
	if (rc != SQLITE_OK ) do_exit(db, err_msg);
    }
    fclose(f);

    rc = sqlite3_exec(db, "COMMIT", 0, 0, &err_msg);
    if (rc != SQLITE_OK ) do_exit(db, err_msg);

    gettimeofday(&tend, NULL);
    double eltime_ms = 1000.0*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("mysql_insert ==> %9.1f ms\n\n", eltime_ms);

    sqlite3_close(db);
}


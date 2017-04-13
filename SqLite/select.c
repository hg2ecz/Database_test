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

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char **argv) {
    // warm up CPU
    for (int i=0; i<1000000000; i++) {
	asm volatile ("nop");
    }

    sqlite3 *db;
//    sqlite3_stmt *res;
    char *err_msg=0;

    int rc = sqlite3_open("dblog.sqlite", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char query[1024];
    sprintf(query, "select * from test where %s=%s\n", argv[1], argv[2]);

    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    rc = sqlite3_exec(db, query, callback, 0, &err_msg);
    if (rc != SQLITE_OK ) do_exit(db, err_msg);

    gettimeofday(&tend, NULL);
    double eltime_ms = 1000.0*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("sqlite_select ==> %9.5f ms\n\n", eltime_ms);

    sqlite3_close(db);
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <postgresql/libpq-fe.h>

void do_exit(PGconn *conn) {
    PQfinish(conn);
    exit(1);
}

int main(int argc, char **argv) {
    if (argc != 3 || (strcmp(argv[1], "keyint") && strcmp(argv[1], "nokeyint"))) {
        printf("Parameters: keyint | nokeyint    and    value\n");
        return -1;
    }
    PGconn *conn = PQconnectdb("user=speedtest password=spdtest dbname=speedtest");

    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection to database failed: %s\n",
            PQerrorMessage(conn));
        do_exit(conn);
    }

    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    char query[1024];
    sprintf(query, "select * from test where %s=%s\n", argv[1], argv[2]);
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");
        PQclear(res);
        do_exit(conn);
    }

    gettimeofday(&tend, NULL);
    double eltime_ms = 1000.0*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("mysql_select ==> %9.5f ms\n\n", eltime_ms);

    int rownum = PQntuples(res);
    for (int i=0; i<rownum; i++) printf("%s;%s;%s\n", PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2));

    PQclear(res);
    PQfinish(conn);

    return 0;
}

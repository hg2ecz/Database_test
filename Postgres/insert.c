#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <postgresql/libpq-fe.h>

void do_exit(PGconn *conn, PGresult *res) {
    fprintf(stderr, "%s\n", PQerrorMessage(conn));    
    PQclear(res);
    PQfinish(conn);    
    exit(1);
}

int main() {
    char query[1000];
    PGconn *conn = PQconnectdb("user=speedtest password=spdtest dbname=speedtest");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        exit(1);
    }

    PGresult *res = PQexec(conn, "DROP TABLE IF EXISTS test");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn, res);
    }
    PQclear(res);

    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn, res);
    }

    sprintf(query, "create table test (keyint integer not null, nokeyint integer not null, str varchar(50))");
    res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn, res); 
    }
    PQclear(res);

    res = PQexec(conn, "CREATE INDEX keyint_idx ON test (keyint)");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn, res); 
    }
    PQclear(res);

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
	PGresult *res = PQexec(conn, query); 
	if (PQresultStatus(res) != PGRES_COMMAND_OK) do_exit(conn, res);
	PQclear(res);
    }
    fclose(f);

    res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn, res);
    }
    PQclear(res);

    gettimeofday(&tend, NULL);
    double eltime_ms = 1000.0*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("mysql_insert ==> %9.1f ms\n\n", eltime_ms);

    PQfinish(conn);
    return 0;
}

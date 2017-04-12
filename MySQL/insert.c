#include <stdio.h>
#include <sys/time.h>
#include <mysql/mysql.h>

int a;

int main(int argc, char **argv) {
    char query[1000];
    if (argc != 2) {
	fprintf(stderr, "Parameter: table engine (InnoDB, MyISAM, Memory\n");
	return -1;
    }
    MYSQL *mysql = mysql_init(NULL);
    if (!mysql_real_connect(mysql, "localhost", "speedtest", "spdtest", "speedtest", 3306, NULL, 0)) {
	fprintf(stderr, "Error: %s\n", mysql_error(mysql));
	return -1;
    }
    mysql_query(mysql, "drop table if exists test");
    sprintf(query, "create table test (keyint int not null, nokeyint int not null, str varchar(50)) engine=%s", argv[1]);
    if (mysql_query(mysql, query)) {
	fprintf(stderr, "Bad engine type. Use InnoDB or MyISAM or Memory as engine parameter.");
	return -1;
    }
    mysql_query(mysql, "alter table test add key(keyint)");

    mysql_autocommit(mysql, 0);

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
	sprintf(query, "INSERT test (keyint, nokeyint, str) values (%s, %s, \"%s\")", field[1], field[1], field[2]);
	mysql_query(mysql, query);
    }
    fclose(f);

    gettimeofday(&tend, NULL);
    double eltime_ms = 1000.0*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("mysql_insert ==> %9.1f ms\n\n", eltime_ms);

    mysql_commit(mysql);
    mysql_close(mysql);
}


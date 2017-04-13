#include <stdio.h>
#include <sys/time.h>
#include <mysql/mysql.h>

int main(int argc, char **argv) {
    if (argc != 3) {
	printf("Parameters: keyint | nokeyint    and    value\n");
	return -1;
    }
    // warm up CPU
    for (int i=0; i<1000000000; i++) {
	asm volatile ("nop");
    }

    MYSQL *mysql = mysql_init(NULL);
    if (!mysql_real_connect(mysql, "localhost", "speedtest", "spdtest", "speedtest", 3306, NULL, 0)) {
	fprintf(stderr, "Error: %s\n", mysql_error(mysql));
	return -1;
    }
    char query[1024];
    sprintf(query, "select * from test where %s=%s\n", argv[1], argv[2]);

    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    mysql_query(mysql, query);
    MYSQL_RES *result = mysql_store_result(mysql);

    gettimeofday(&tend, NULL);
    double eltime_ms = 1000.0*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("mysql_select ==> %9.5f ms\n\n", eltime_ms);

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) { 
	for(int i = 0; i < num_fields; i++) { 
	    printf("%s ", row[i] ? row[i] : "NULL"); 
        } 
	printf("\n"); 
    }

    mysql_free_result(result);
    mysql_close(mysql);
}


#include "sqlite3.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"

unsigned int randr(unsigned int min, unsigned int max) {
    double scaled = (double) rand() / RAND_MAX;

    return (max - min + 1) * scaled + min;
}

void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

void measure_query_time(sqlite3* db, int i, char* field){
    clock_t start = clock(), diff;
    char sql[50];
    sprintf(sql, "select * from fun order by %s", field);
    sqlite3_exec(db, sql, 0, 0, 0);
    diff = clock() - start;
    long msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("%d %ld\n", i, msec / 1000, msec % 1000);
}

int main(int argc, char *argv[]) {
    sqlite3 *db;
    sqlite3_open_v2(":memory", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    sqlite3_exec(db, "drop table if exists fun;", NULL, NULL, NULL);
    sqlite3_exec(db, "create table fun(id num, randstr text, randint num, randreal real);", NULL, NULL, NULL);
    srand(time(NULL));
    int rows = 10000000;
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    for (int i = 1; 1==1; i++) {

        long long int randint = ((long long) rand() << 32) | rand();
        randint = (rand() % 2)? randint: -randint;
        double randreal = -1. + rand() / (RAND_MAX / 2.);
        int len = randr(1, 11);
        char randstr[len];
        rand_str(randstr, len);
        char buffer[300];
        sprintf(buffer, "insert into fun values(%d,'%s',%lld,%lf)",
                i, randstr, randint, randreal);
        sqlite3_exec(db, buffer, NULL, NULL, NULL);
        if (i % atoi(argv[1]) == 0){
            sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
            measure_query_time(db, i, argv[2]);
            sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
        }

    }
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);


}


#include "sqlite3.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
#include "cputime.c"


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
    double start = getCPUTime(), diff;
    char sql[50];
    sprintf(sql, "select * from fun order by %s", field);
    sqlite3_exec(db, sql, 0, 0, 0);
    diff = getCPUTime() - start;
    printf("%d\t%lf\n", i, diff);
}
int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    sqlite3 *db;
    sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA automatic_index=false", NULL, NULL, NULL);

    sqlite3_exec(db, "drop table if exists fun;", NULL, NULL, NULL);
    sqlite3_exec(db, "create table fun(id num, randstr text, randint num, randreal real);", NULL, NULL, NULL);
    srand(time(NULL));
    char sql[] = "insert into fun values(?1,?2,?3,?4)";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    for (int i = 1; i < 2000000000; i++) {

        long long int randint = ((long long) rand() << 32) | rand();
        randint = (rand() % 2)? randint: -randint;
        double randreal = -1. + rand() / (RAND_MAX / 2.);
        int len = randr(1, 11);
        char randstr[len];
        rand_str(randstr, len);
        sqlite3_bind_int(stmt, 1, i);
        sqlite3_bind_text(stmt, 2, randstr, len, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 3, randint);
        sqlite3_bind_double(stmt, 4, randreal);
        sqlite3_step(stmt);
        if (argc < 4 || i > atoi(argv[3])){
            if (i % atoi(argv[1]) == 0){
                sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
                measure_query_time(db, i, argv[2]);
                sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
            }
        }
        sqlite3_reset(stmt);

    }
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);


}


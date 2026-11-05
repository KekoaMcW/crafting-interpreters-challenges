#include <stdio.h>
#include <string.h>
#include <time.h>

#include "benchmark.h"
#include "memory.h"
#include "object.h"
#include "table.h"
#include "vm.h"

#define N 100000

static ObjString* keys[N];
static ObjString* missing[N];

static double seconds(clock_t start) {
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

static void makeKeys(const char* prefix) {
    char buffer[300];
    for (int i = 0; i < N; i++) {
        int length = sprintf(buffer, "%s%d", prefix, i);
        keys[i] = copyString(buffer, length);
        length = sprintf(buffer, "%smissing%d", prefix, i);
        missing[i] = copyString(buffer, length);
    }
}

static void insertAndFind(const char* name) {
    Table table;
    initTable(&table);
    Value value;

    clock_t start = clock();
    for (int i = 0; i < N; i++) tableSet(&table, keys[i], NUMBER_VAL(i));
    double insertTime = seconds(start);

    start = clock();
    for (int i = 0; i < N; i++) tableGet(&table, keys[i], &value);
    double findTime = seconds(start);

    start = clock();
    for (int i = 0; i < N; i++) tableGet(&table, missing[i], &value);
    double missTime = seconds(start);

    printf("%-14s insert %.4fs  find %.4fs  miss %.4fs\n",
           name, insertTime, findTime, missTime);
    freeTable(&table);
}

static void deleteHeavy() {
    Table table;
    initTable(&table);
    Value value;

    clock_t start = clock();
    for (int round = 0; round < 10; round++) {
        for (int i = 0; i < N; i++) tableSet(&table, keys[i], NUMBER_VAL(i));
        for (int i = 0; i < N; i++) tableDelete(&table, keys[i]);
    }
    double churnTime = seconds(start);

    start = clock();
    for (int i = 0; i < N; i++) tableGet(&table, missing[i], &value);
    double missTime = seconds(start);

    printf("%-14s churn  %.4fs  miss after deletes %.4fs\n",
           "delete heavy", churnTime, missTime);
    freeTable(&table);
}

static void smallTable() {
    Table table;
    initTable(&table);
    Value value;
    for (int i = 0; i < 8; i++) tableSet(&table, keys[i], NUMBER_VAL(i));

    clock_t start = clock();
    for (int round = 0; round < N; round++) {
        for (int i = 0; i < 8; i++) tableGet(&table, keys[i], &value);
    }

    printf("%-14s %d lookups %.4fs\n", "small table", N * 8,
           seconds(start));
    freeTable(&table);
}

void runBenchmarks() {
    makeKeys("k");
    insertAndFind("short keys");
    deleteHeavy();
    smallTable();

    char prefix[260];
    memset(prefix, 'x', 255);
    prefix[255] = '\0';
    makeKeys(prefix);
    insertAndFind("long keys");
}

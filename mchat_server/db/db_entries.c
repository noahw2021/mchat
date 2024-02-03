//
//  db_entries.c
//  mchat_server
//
//  Created by Noah Wooten on 2/3/24.
//

#include "db.h"

HENTRY DbGetEntryByIterator(HBASE Base, HTABLE Table, WORD64 Iterator) {
    return 0;
}

HENTRY DbGetEntryByIndex(HBASE Base, HTABLE Table, WORD64 Index, int i) {
    return 0;
}

int DbGetEntryWithIndexCount(HBASE Base, HTABLE Table, WORD64 Index) {
    return 0;
}

void* DbEntryRead(HBASE Base, HTABLE Table, HENTRY Entry) {
    return NULL;
}

void DbEntryWrite(HBASE Base, HTABLE Table, HENTRY Entry, void* Buffer) {
    return;
}

void DbEntryDelete(HBASE Base, HTABLE Table, HENTRY Entry) {
    return;
}

HENTRY DbEntryCreate(HBASE Base, HTABLE Table, void* Buffer) {
    return 0;
}

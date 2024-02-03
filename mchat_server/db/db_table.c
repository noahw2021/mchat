//
//  db_table.c
//  mchat_server
//
//  Created by Noah Wooten on 2/3/24.
//

#include "db.h"

HTABLE DbGetTableByName(HBASE Base, const char* Name) {
    return 0;
}

HTABLE DbGetTableByIterator(HBASE Base, int i) {
    return 0;
}

char* DbGetTableNameByIterator(HBASE Base, int i) {
    return NULL;
}

int DbGetTableCount(HBASE Base) {
    return 0;
}

HTABLE DbCreateTable(HBASE Base, const char* Name, WORD64 StructSize) {
    return 0;
}

void DbTableSetIndex(HBASE Base, WORD64 IndexOffset) {
    return;
}

//
//  db_base.c
//  mchat_server
//
//  Created by Noah Wooten on 2/3/24.
//

#include "db.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

HBASE DbCreate(const char* Name) {
    pthread_mutex_lock(&DbCtx->BasesMutex);
    if (!DbCtx->Bases) {
        DbCtx->Bases = malloc(sizeof(DB_CTX));
    } else {
        DbCtx->Bases = realloc(DbCtx->Bases, sizeof(DB_CTX) *
            (DbCtx->BaseCount + 1));
    }
    
    PDB_BASE NewBase = DbCtx->Bases[DbCtx->BaseCount];
    DbCtx->BaseCount++;
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    
    pthread_mutex_init(&NewBase->TablesMutex, NULL);
    
    return (DbCtx->BaseCount - 1);
}

HBASE DbOpen(const char* Name) {
    return 0;
}

void DbClose(HBASE Base) {
    return;
}

//
//  db_tables.c
//  mchat_server
//
//  Created by Noah Wooten on 1/31/24.
//

#include "db.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

WORD64 DbBaseCreateTable(const char* TableName) {
    pthread_mutex_lock(&DbCtx->BasesMutex);
    PDB_BASE ActiveBase = &DbCtx->Bases[DbCtx->ActiveWorkingBase];
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    
    
}

void DbBaseDeleteTable(WORD64 TableId) {
    
}

void DbBaseCreateField(WORD64 TableId, const char* Name, int Type) {
    
}

WORD64 DbBaseDeleteField(WORD64 TableId, WORD64 FieldId) {
    
}

WORD64 DbBaseGetFieldByName(WORD64 TableId, const char* Name) {
    
}

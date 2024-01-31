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
    if (!DbCtx)
        return 0xFFFFFFFFFFFFFFFF;
    
    pthread_mutex_lock(&DbCtx->BasesMutex);
    PDB_BASE ActiveBase = DbCtx->Bases[DbCtx->ActiveWorkingBase];
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    
    pthread_mutex_lock(&ActiveBase->Access);
    
    PDB_TABLE PreviousTable2 = NULL;
    
    if (!ActiveBase->Tables) {
        ActiveBase->Tables = malloc(sizeof(DB_TABLE*));
        ActiveBase->FirstTableOffset = ActiveBase->NextFreeChunk;
    } else {
        ActiveBase->Tables = realloc(ActiveBase->Tables,
            sizeof(DB_TABLE*) * (ActiveBase->TableCount + 1));
        
        PDB_TABLE PreviousTable = 
            ActiveBase->Tables[ActiveBase->TableCount - 1];
        PreviousTable2 = PreviousTable;
        
        // update the next
        pthread_mutex_lock(&PreviousTable->Mutex);
        
        pthread_mutex_lock(&ActiveBase->NextFreeChunkMutex);
        PreviousTable->Next = ActiveBase->NextFreeChunk;
        
        pthread_mutex_lock(&ActiveBase->FileMutex);
        
        fseek(ActiveBase->FilePointer, PreviousTable->Me, SEEK_SET);
        fwrite(PreviousTable, SZ__DB_TABLE, 1, SEEK_SET);
        
        pthread_mutex_unlock(&ActiveBase->FileMutex);
        pthread_mutex_unlock(&PreviousTable->Mutex);
    }
    
    pthread_mutex_unlock(&ActiveBase->Access);
    
    PDB_TABLE* NewTable2 = &ActiveBase->Tables[ActiveBase->TableCount];
    *NewTable2 = malloc(sizeof(DB_TABLE));
    PDB_TABLE NewTable = *NewTable2;
    ActiveBase->TableCount++;
    memset(NewTable, 0, sizeof(DB_TABLE));
    
    NewTable->Me = ActiveBase->NextFreeChunk;
    ActiveBase->NextFreeChunk += SZ__DB_TABLE;
    pthread_mutex_unlock(&ActiveBase->NextFreeChunkMutex);
   
    if (PreviousTable2) {
        pthread_mutex_lock(&PreviousTable2->Mutex);
        NewTable->Previous = PreviousTable2->Me;
        pthread_mutex_unlock(&PreviousTable2->Mutex);
    }
    
    pthread_mutex_unlock(&ActiveBase->Access);
    pthread_mutex_init(&NewTable->Mutex, NULL);
    
    time(&NewTable->Created);
    pthread_mutex_lock(&ActiveBase->Access);
    NewTable->TableId = ActiveBase->IteratorBase++;
    pthread_mutex_unlock(&ActiveBase->Access);
    
    strcpy(NewTable->TableName, TableName);
    return (NewTable->TableId);
}

void DbBaseDeleteTable(WORD64 TableId) {
    
}

void DbBaseCreateField(WORD64 TableId, const char* Name, int Type) {
    
}

WORD64 DbBaseDeleteField(WORD64 TableId, WORD64 FieldId) {
    
}

WORD64 DbBaseGetFieldByName(WORD64 TableId, const char* Name) {
    
}

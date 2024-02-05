//
//  db_table.c
//  mchat_server
//
//  Created by Noah Wooten on 2/3/24.
//

#include "db.h"
#include <string.h>
#include <stdlib.h>

HTABLE DbGetTableByName(HBASE Base, const char* Name) {
    PDB_BASE ThisBase = DbCtx->Bases[Base];
    
    pthread_mutex_lock(&ThisBase->TablesMutex);
    for (int i = 0; i < ThisBase->TableCount; i++) {
        PDB_TABLE ThisTable = ThisBase->Tables[i];
        pthread_mutex_unlock(&ThisBase->TablesMutex);
        
        if (!strcmp(ThisTable->Name, Name)) {
            return i;
        }
        
        pthread_mutex_lock(&ThisBase->TablesMutex);
    }
    
    pthread_mutex_unlock(&ThisBase->TablesMutex);
    return -1;
}

HTABLE DbGetTableByIterator(HBASE Base, int i) {
    HTABLE Return;
    
    pthread_mutex_lock(&DbCtx->BasesMutex);
    if (i >= DbCtx->BaseCount) {
        Return = (DbCtx->BaseCount - 1);
    } else {
        Return = i;
    }
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    
    return Return;
}

char* DbGetTableNameByIterator(HBASE Base, int i) {
    char* NameBuffer = malloc(128);
    
    pthread_mutex_lock(&DbCtx->BasesMutex);
    PDB_BASE ThisBase = DbCtx->Bases[Base];
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    pthread_mutex_lock(&ThisBase->TablesMutex);
    PDB_TABLE ThisTable = ThisBase->Tables[i];
    pthread_mutex_unlock(&ThisBase->TablesMutex);
    strlcpy(NameBuffer, ThisTable->Name, 128);
    
    return NameBuffer;
}

WORD64 DbGetTableCount(HBASE Base) {
    return DbCtx->BaseCount;
}

HTABLE DbCreateTable(HBASE Base, const char* Name, WORD64 StructSize) {
    pthread_mutex_lock(&DbCtx->BasesMutex);
    PDB_BASE ThisBase = DbCtx->Bases[Base];
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    
    pthread_mutex_lock(&ThisBase->TablesMutex);
    if (!ThisBase->Tables) {
        ThisBase->Tables = malloc(sizeof(DB_TABLE*));
        ThisBase->Tables[0] = malloc(sizeof(DB_TABLE));
    } else {
        ThisBase->Tables = realloc(ThisBase->Tables,
            sizeof(DB_TABLE) * (ThisBase->TableCount + 1));
        ThisBase->Tables[ThisBase->TableCount] = malloc(sizeof(DB_TABLE));
    }
    
    PDB_TABLE NewTable = ThisBase->Tables[ThisBase->TableCount];
    ThisBase->TableCount++;
    pthread_mutex_init(&NewTable->EntriesMutex, NULL);
    pthread_mutex_init(&NewTable->IndicesMutex, NULL);
    strcpy(NewTable->Name, Name);
    
    pthread_mutex_lock(&ThisBase->TablesMutex);
    NewTable->Me = ThisBase->NextFreeSection;
    if (!ThisBase->FirstTableOffset)
        ThisBase->FirstTableOffset = NewTable->Me;
    ThisBase->NextFreeSection += _SZ_DB_TABLE;
    pthread_mutex_unlock(&ThisBase->TablesMutex);
    
    PDB_TABLE Previous = NULL;
    if ((ThisBase->TableCount - 1))
        Previous = ThisBase->Tables[ThisBase->TableCount - 2];
    
    if (Previous) {
        pthread_mutex_lock(&ThisBase->FileMutex);
        Previous->Next = NewTable->Me;
        fseek(ThisBase->FilePointer, Previous->Me, SEEK_SET);
        fwrite(Previous, _SZ_DB_TABLE, 1, ThisBase->FilePointer);
        pthread_mutex_unlock(&ThisBase->FileMutex);
        NewTable->Previous = Previous->Me;
    }

    NewTable->StructSize = StructSize;
    
    pthread_mutex_lock(&ThisBase->FileMutex);
    fseek(ThisBase->FilePointer, NewTable->Me, SEEK_SET);
    fwrite(NewTable, _SZ_DB_TABLE, 1, ThisBase->FilePointer);
    pthread_mutex_unlock(&ThisBase->FileMutex);
    
    pthread_mutex_unlock(&ThisBase->TablesMutex);
    
    return (ThisBase->TableCount - 1);
}

void DbTableSetIndex(HBASE Base, WORD64 IndexOffset) {
    return;
}

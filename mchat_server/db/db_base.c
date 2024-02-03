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
        DbCtx->Bases = malloc(sizeof(DB_BASE**));
        DbCtx->Bases[DbCtx->BaseCount] = malloc(sizeof(DB_BASE));
    } else {
        DbCtx->Bases = realloc(DbCtx->Bases, sizeof(DB_BASE*) *
            (DbCtx->BaseCount + 1));
        DbCtx->Bases[DbCtx->BaseCount] = malloc(sizeof(DB_BASE));
    }
    
    PDB_BASE NewBase = DbCtx->Bases[DbCtx->BaseCount];
    DbCtx->BaseCount++;
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    
    pthread_mutex_init(&NewBase->TablesMutex, NULL);
    pthread_mutex_init(&NewBase->FileMutex, NULL);
    
    FILE* Physical = fopen(Name, "rb+");
    NewBase->FilePointer = Physical;
    
    fseek(Physical, 0, SEEK_SET);
    fwrite(NewBase, sizeof(DB_BASE), 1, Physical);
    
    return (DbCtx->BaseCount - 1);
}

HBASE DbOpen(const char* Name) {
    FILE* Physical = fopen(Name, "rb+");
    if (!Physical)
        return -1;
    
    pthread_mutex_lock(&DbCtx->BasesMutex);
    if (!DbCtx->Bases) {
        DbCtx->Bases = malloc(sizeof(DB_BASE*));
        DbCtx->Bases[DbCtx->BaseCount] = malloc(sizeof(DB_BASE));
    } else {
        DbCtx->Bases = realloc(DbCtx->Bases, sizeof(DB_BASE*) *
            (DbCtx->BaseCount + 1));
        DbCtx->Bases[DbCtx->BaseCount] = malloc(sizeof(DB_BASE));
    }
    
    PDB_BASE NewBase = DbCtx->Bases[DbCtx->BaseCount];
    DbCtx->BaseCount++;
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    
    pthread_mutex_init(&NewBase->FileMutex, NULL);
    pthread_mutex_init(&NewBase->TablesMutex, NULL);
    NewBase->FilePointer = Physical;
    
    // read the file
    pthread_mutex_lock(&NewBase->FileMutex);
    fseek(NewBase->FilePointer, 0, SEEK_SET);
    fread(NewBase, _SZ_DB_BASE, 1, NewBase->FilePointer);
    pthread_mutex_unlock(&NewBase->FileMutex);
    
    // create the tables
    WORD64 NextFileOffset = NewBase->FirstTableOffset;
    while (NextFileOffset) {
        pthread_mutex_lock(&NewBase->TablesMutex);
        if (!NewBase->Tables) {
            NewBase->Tables = malloc(sizeof(DB_TABLE*));
            NewBase->Tables[NewBase->TableCount] = malloc(sizeof(DB_TABLE));
        } else {
            NewBase->Tables = realloc(NewBase->Tables, sizeof(DB_TABLE*) *
                (NewBase->TableCount - 1));
            NewBase->Tables[NewBase->TableCount] = malloc(sizeof(DB_TABLE));
        }
        
        PDB_TABLE NewTable = NewBase->Tables[NewBase->TableCount];
        NewBase->TableCount++;
        pthread_mutex_unlock(&NewBase->TablesMutex);
        
        pthread_mutex_lock(&NewBase->FileMutex);
        fseek(NewBase->FilePointer, NextFileOffset, SEEK_SET);
        fread(NewTable, _SZ_DB_TABLE, 1, NewBase->FilePointer);
        pthread_mutex_unlock(&NewBase->FileMutex);
        
        
        // create the entries
        pthread_mutex_init(&NewTable->EntriesMutex, NULL);
        WORD64 NextEntry = NewTable->FirstEntryOffset;
        while (NextEntry) {
            if (!NewTable->Entries) {
                NewTable->Entries = malloc(sizeof(DB_ENTRY**));
                NewTable->Entries[NewTable->EntryCount] = 
                    malloc(sizeof(DB_ENTRY));
            } else {
                NewTable->Entries = realloc(NewTable->Entries, 
                    sizeof(DB_ENTRY**) * sizeof(NewTable->EntryCount + 1));
                NewTable->Entries[NewTable->EntryCount] =
                    malloc(sizeof(DB_ENTRY));
            }
            
            PDB_ENTRY NewEntry = NewTable->Entries[NewTable->EntryCount];
            NewTable->EntryCount++;
            pthread_mutex_init(&NewEntry->EntryMutex, NULL);
            
            pthread_mutex_lock(&NewBase->FileMutex);
            fseek(NewBase->FilePointer, NextEntry, SEEK_SET);
            fread(NewEntry, _SZ_DB_ENTRY, 1, NewBase->FilePointer);
            pthread_mutex_unlock(&NewBase->FileMutex);
            
            NextEntry = NewEntry->Next;
        }
        
        // create the indices
        pthread_mutex_init(&NewTable->EntriesMutex, NULL);
        WORD64 NextIndex = NewTable->FirstIndexOffset;
        while (NextIndex) {
            if (!NewTable->Indices) {
                NewTable->Indices = malloc(sizeof(DB_INDEX**));
                NewTable->Indices[NewTable->IndexCount] =
                    malloc(sizeof(DB_INDEX));
            } else {
                NewTable->Indices = realloc(NewTable->Indices,
                    sizeof(DB_INDEX**) * (NewTable->IndexCount + 1));
                NewTable->Indices[NewTable->IndexCount] =
                    malloc(sizeof(DB_INDEX));
            }
            
            PDB_INDEX NewIndex = NewTable->Indices[NewTable->IndexCount - 1];
            NewTable->IndexCount++;
            
            pthread_mutex_init(&NewIndex->IndexMutex, NULL);
            
            pthread_mutex_lock(&NewBase->FileMutex);
            fseek(NewBase->FilePointer, NextIndex, SEEK_SET);
            fread(NewIndex, _SZ_DB_ENTRY, 1, NewBase->FilePointer);
            pthread_mutex_unlock(&NewBase->FileMutex);
            
            NextIndex = NewIndex->Next;
        }
    }
    
    return (DbCtx->BaseCount - 1);
}

void DbClose(HBASE Base) {
    PDB_BASE ThisBase = DbCtx->Bases[Base];
    
    for (int t = 0; t < ThisBase->TableCount; t++) {
        PDB_TABLE ThisTable = ThisBase->Tables[t];
        
        for (int e = 0; e < ThisTable->EntryCount; e++) {
            PDB_ENTRY ThisEntry = ThisTable->Entries[e];
            
            if (ThisEntry->EntryValue)
                free(ThisEntry->EntryValue);
            
            pthread_mutex_destroy(&ThisEntry->EntryMutex);
            free(ThisEntry);
        }
        
        if (ThisTable->Entries)
            free(ThisTable->Entries);
        
        for (int i = 0; i < ThisTable->IndexCount; i++) {
            PDB_INDEX ThisIndex = ThisTable->Indices[i];
            
            pthread_mutex_destroy(&ThisIndex->IndexMutex);
            free(ThisIndex);
        }
        
        if (ThisTable->Indices)
            free(ThisTable->Indices);
        
        pthread_mutex_destroy(&ThisTable->EntriesMutex);
        pthread_mutex_destroy(&ThisTable->IndicesMutex);
        free(ThisTable);
    }
    
    pthread_mutex_destroy(&ThisBase->TablesMutex);
    free(ThisBase);
}

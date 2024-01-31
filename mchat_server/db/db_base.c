//
//  db_base.c
//  mchat_server
//
//  Created by Noah Wooten on 1/29/24.
//

#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

WORD64 DbCreateBase(const char* FileName) {
    return 0;
}

WORD64 DbOpenBase(const char* FileName) {
    if (!DbCtx)
        return 0xFFFFFFFFFFFFFFFF;
    
    FILE* InputFile = fopen(FileName, "rb+");
    if (!InputFile)
        return 0xFFFFFFFFFFFFFFFF;
    
    pthread_mutex_lock(&DbCtx->BasesMutex);
    if (!DbCtx->Bases) {
        DbCtx->Bases = malloc(sizeof(DB_BASE*));
    } else {
        DbCtx->Bases = realloc(DbCtx->Bases,
            sizeof(DB_BASE*) * (DbCtx->BaseCount + 1));
    }
    
    PDB_BASE* NewBase2 = &DbCtx->Bases[DbCtx->BaseCount + 1];
    *NewBase2 = malloc(sizeof(DB_BASE));
    PDB_BASE NewBase = *NewBase2;
    DbCtx->BaseCount++;
    memset(NewBase, 0, sizeof(DB_BASE));
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    
    fseek(InputFile, 0, SEEK_SET);
    fread(NewBase, SZ__DB_BASE, 1, InputFile);
    NewBase->TableCount = 0; // must init at zero.
    
    NewBase->LocalBaseId = DbCtx->IteratorBase++;
    pthread_mutex_init(&NewBase->NextFreeChunkMutex, NULL);
    pthread_mutex_init(&NewBase->FileMutex, NULL);
    pthread_mutex_init(&NewBase->Access, NULL);
    
    // load all tables and fields
    WORD64 NextTableOffset = NewBase->FirstTableOffset;
    while (NextTableOffset) {
        if (!NewBase->Tables) {
            NewBase->Tables = malloc(sizeof(DB_TABLE));
        } else {
            NewBase->Tables = realloc(NewBase->Tables,
                sizeof(DB_TABLE) * (NewBase->TableCount));
        }
        
        PDB_TABLE* NewTable2 = &NewBase->Tables[NewBase->TableCount];
        *NewTable2 = malloc(sizeof(DB_TABLE));
        PDB_TABLE NewTable = *NewTable2;
        NewBase->TableCount++;
        
        fseek(InputFile, NextTableOffset, SEEK_SET);
        fread(NewTable, SZ__DB_TABLE, 1, InputFile);
        NewTable->EntryCount = 0;
        NewTable->FieldCount = 0;
        NewTable->EntryIndexCount = 0;
        
        WORD64 NextFieldOffset = NewTable->FirstFieldOffset;
        while (NextFieldOffset) {
            if (!NewTable->Fields) {
                NewTable->Fields = malloc(sizeof(DB_TABLE));
            } else {
                NewTable->Fields = realloc(NewTable->Fields,
                    sizeof(DB_TABLE) * (NewTable->FieldCount + 1));
            }
            
            PDB_FIELD NewField = &NewTable->Fields[NewTable->FieldCount];
            NewTable->FieldCount++;
            
            fseek(InputFile, NextFieldOffset, SEEK_SET);
            fread(NewField, SZ__DB_FIELD, 1, InputFile);
            
            NextFieldOffset = NewField->Next;
        }
        
        WORD64 NextIndexOffset = NewTable->FirstEntryIndex;
        while (NextIndexOffset) {
            if (!NewTable->EntryIndices) {
                NewTable->EntryIndices = malloc(sizeof(DB_ENTRY_INDEX));
            } else {
                NewTable->EntryIndices = realloc(NewTable->EntryIndices,
                    sizeof(DB_ENTRY_INDEX) * (NewTable->EntryIndexCount));
            }
            
            PDB_ENTRY_INDEX EntryIndex =
                &NewTable->EntryIndices[NewTable->EntryIndexCount];
            NewTable->EntryIndexCount++;
            
            fseek(InputFile, NewTable->FirstEntryIndex, SEEK_SET);
            fread(EntryIndex, SZ__DB_ENTRY_INDEX, 1, InputFile);
            
            EntryIndex->IndexMembers = malloc(sizeof(DB_ENTRY_INDEX_MEMBER) *
                (EntryIndex->IndexMemberSize));
            fseek(InputFile, EntryIndex->FirstIndexMember, SEEK_SET);
            fread(EntryIndex->IndexMembers, SZ__DB_ENTRY_INDEX_MEMBER,
                EntryIndex->IndexMemberSize, InputFile);
            
            NextIndexOffset = EntryIndex->Next;
        }
        
        NextTableOffset = NewTable->Next;
    }
    
    return (NewBase->LocalBaseId);
}

void DbCloseBase(WORD64 BaseId) {
    if (!DbCtx)
        return;
    
    PDB_BASE ThisBase = NULL;
    WORD64 PhysIt = 0;
    
    pthread_mutex_lock(&DbCtx->BasesMutex);
    for (int i = 0; i < DbCtx->BaseCount; i++) {
        PDB_BASE ActiveBase = DbCtx->Bases[i];
        
        if (ActiveBase->LocalBaseId == BaseId) {
            ThisBase = ActiveBase;
            PhysIt = i;
            break;
        }
    }
    
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    if (!ThisBase)
        return;
    
    pthread_mutex_lock(&ThisBase->Access);
    
    for (int i = 0; i < ThisBase->TableCount; i++) {
        PDB_TABLE ThisTable = ThisBase->Tables[i];
        
        for (int e = 0; e < ThisTable->EntryCount; e++) {
            PDB_ENTRY ThisEntry = &ThisTable->Entries[e];
            
            for (int ef = 0; ef < ThisEntry->EntryFieldCount; ef++) {
                PDB_ENTRY_FIELD ThisEntryField = &ThisEntry->EntryFields[ef];
                
                if (ThisEntryField->ResolvedData.Buffer)
                    free(ThisEntryField->ResolvedData.Buffer);
            }
            
            if (ThisEntry->EntryFields)
                free(ThisEntry->EntryFields);
        }
        
        if (ThisTable->Entries)
            free(ThisTable->Entries);
        if (ThisTable->Fields)
            free(ThisTable->Fields);
        
        for (int ei = 0; ei < ThisTable->EntryIndexCount; ei++) {
            PDB_ENTRY_INDEX ThisEntryIndex = &ThisTable->EntryIndices[ei];
            
            if (ThisEntryIndex->IndexMembers)
                free(ThisEntryIndex->IndexMembers);
        }
        
        if (ThisTable->EntryIndexCount)
            free(ThisTable->EntryIndices);
        
        free(ThisTable);
    }
    
    if (ThisBase->Tables)
        free(ThisBase->Tables);
   
    pthread_mutex_destroy(&ThisBase->Access);
    
    pthread_mutex_lock(&DbCtx->BasesMutex);
    void* NewBases = DbuRemoveEntry(DbCtx->Bases, PhysIt, sizeof(DB_BASE*),
        DbCtx->BaseCount);
    void* OldBase = DbCtx->Bases;
    DbCtx->Bases = NewBases;
    pthread_mutex_unlock(&DbCtx->BasesMutex);
    free(OldBase);
    
    return;
}

void DbSetWorkingBase(WORD64 BaseId) {
    if (!DbCtx)
        return;
    
    DbCtx->ActiveWorkingBase = BaseId;
    return;
}

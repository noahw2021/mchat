//
//  db.h
//  mchat_server
//
//  Created by Noah Wooten on 2/3/24.
//

#ifndef db_h
#define db_h

#include "../types.h"
#include <pthread.h>
#include <stdio.h>

void DbInit(void);
void DbShutdown(void);

typedef unsigned long long HBASE, *PHBASE;
typedef unsigned long long HTABLE, *PHTABLE;
typedef unsigned long long HENTRY, *PHENTRY;

HBASE DbCreate(const char* Name);
HBASE DbOpen(const char* Name);
void DbClose(HBASE Base);

HTABLE DbGetTableByName(HBASE Base, const char* Name);
HTABLE DbGetTableByIterator(HBASE Base, int i);
char*  DbGetTableNameByIterator(HBASE Base, int i);
int    DbGetTableCount(HBASE Base);

/*
 Tables can have indexes which are unsigned long longs and can be used
 to quickly find entries.
 
 Use DbGetEntryByIndex to get these entries, and the 'i' argument is for
 if multiple entries have the same index. Use DbGetEntryWithIndexCount
 to determine how many entries have this index.
 */

void DbInit(void);
void DbShutdown(void);

HTABLE DbCreateTable(HBASE Base, const char* Name, WORD64 StructSize);
void DbTableSetIndex(HBASE Base, WORD64 IndexOffset);

HENTRY DbGetEntryByIterator(HBASE Base, HTABLE Table, WORD64 Iterator);
HENTRY DbGetEntryByIndex(HBASE Base, HTABLE Table, WORD64 Index, int i);
int    DbGetEntryWithIndexCount(HBASE Base, HTABLE Table, WORD64 Index);

void* DbEntryRead(HBASE Base, HTABLE Table, HENTRY Entry);
void  DbEntryWrite(HBASE Base, HTABLE Table, HENTRY Entry, void* Buffer);
void  DbEntryDelete(HBASE Base, HTABLE Table, HENTRY Entry);
HENTRY DbEntryCreate(HBASE Base, HTABLE Table, void* Buffer);

typedef union _DB_ENTRY_VALUE {
    void* Buffer;
    
    WORD64* Data_WORD64;
    char* Data_STRING;
}DB_ENTRY_VALUE, *PDB_ENTRY_VALUE;

typedef struct _DB_INDEX {
    WORD64 Previous, Me, Next;
    
    WORD64 IndexValue[1024];
    WORD64 IndexPointer[1024];
    
    // do not include to save
    pthread_mutex_t IndexMutex;
}DB_INDEX, *PDB_INDEX;
#define _SZ_DB_INDEX sizeof(DB_INDEX) - sizeof(pthread_mutex_t)

typedef struct _DB_ENTRY {
    WORD64 Previous, Me, Next;
    
    WORD64 DataPtr;
    WORD64 QuickIndexValue;
    
    // do not include to save
    PDB_ENTRY_VALUE EntryValue;
    pthread_mutex_t EntryMutex;
}DB_ENTRY, *PDB_ENTRY;
#define _SZ_DB_ENTRY sizeof(DB_ENTRY) - sizeof(PDB_ENTRY_VALUE) - \
    sizeof(WORD64) - sizeof(pthread_mutex_t) - sizeof(FILE*) - sizeof(pthread_mutex_t)

typedef struct _DB_TABLE {
    WORD64 Previous, Me, Next;
    
    char Name[128];
    WORD64 FirstEntryOffset;
    WORD64 FirstIndexOffset;
    
    // do not include to save
    PDB_ENTRY* Entries;
    PDB_INDEX* Indices;
    WORD64 EntryCount;
    WORD64 IndexCount;
    pthread_mutex_t EntriesMutex;
    pthread_mutex_t IndicesMutex;
}DB_TABLE, *PDB_TABLE;
#define _SZ_DB_TABLE sizeof(DB_TABLE) - sizeof(PDB_ENTRY*) - \
    sizeof(PDB_INDEX*) - sizeof(WORD64) - sizeof(WORD64) - sizeof(pthread_mutex_t) - sizeof(pthread_mutex_t)

typedef struct _DB_BASE {
    WORD64 FirstTableOffset;
    
    // do not include to save
    PDB_TABLE* Tables;
    WORD64 TableCount;
    pthread_mutex_t TablesMutex;
    FILE* FilePointer;
    pthread_mutex_t FileMutex;
}DB_BASE, *PDB_BASE;
#define _SZ_DB_BASE sizeof(DB_BASE) - sizeof(PDB_TABLE*) - sizeof(WORD64) - sizeof(pthread_mutex_t)

typedef struct _DB_CTX {
    PDB_BASE* Bases;
    WORD64 BaseCount;
    
    pthread_mutex_t BasesMutex;
}DB_CTX, *PDB_CTX;
extern PDB_CTX DbCtx;

#endif /* db_h */

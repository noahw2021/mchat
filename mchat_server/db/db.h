//
//  db.h
//  mchat_server
//
//  Created by Noah Wooten on 2/3/24.
//

#ifndef db_h
#define db_h

#include "../types.h"

void DbInit(void);
void DbShutdown(void);

typedef unsigned long long HBASE, *PHBASE;
typedef unsigned long long HTABLE, *PHTABLE;
typedef unsigned long long HENTRY, *PHENTRY;

HBASE DbCreate(const char* Name);
HBASE DbOpen(const char* Name);

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

typedef struct _DB_INDEX_MEMBER {
    WORD64 IndexValue;
    WORD64 EntryOffset;
}DB_INDEX_MEMBER, *PDB_INDEX_MEMBER;

typedef struct _DB_INDEX {
    WORD64 Previous, Me, Next;
    
    WORD64 FirstMemberOffset;
    WORD64 MemberSize;
    
    // do not include to save
    PDB_INDEX_MEMBER IndexMembers;
    WORD64 MemberCount;
}DB_INDEX, *PDB_INDEX;
#define _SZ_DB_INDEX sizeof(DB_INDEX) - sizeof(PDB_INDEX_MEMBER) - \
    sizeof(WORD64)

typedef struct _DB_ENTRY {
    WORD64 Previous, Me, Next;
    
    WORD64 DataPtr;
    WORD64 QuickIndexValue;
    
    // do not include to save
    PDB_ENTRY_VALUE EntryValues;
    WORD64 EntryValueCount;
}DB_ENTRY, *PDB_ENTRY;
#define _SZ_DB_ENTRY sizeof(DB_ENTRY) - sizeof(PDB_ENTRY_VALUE) - \
    sizeof(WORD64)

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
}DB_TABLE, *PDB_TABLE;
#define _SZ_DB_TABLE sizeof(DB_TABLE) - sizeof(PDB_ENRTY*) - \
    sizeof(PDB_INDEX*) - sizeof(WORD64) - sizeof(WORD64)

typedef struct _DB_BASE {
    WORD64 FirstTableOffset;
    
    // do not include to save
    PDB_TABLE* Tables;
    WORD64 TableCount;
}DB_BASE, *PDB_BASE;
#define _SZ_DB_BASE sizeof(DB_BASE) - sizeof(PDB_TABLE*) - sizeof(WORD64)

typedef struct _DB_CTX {
    PDB_BASE Bases;
    WORD64 BaseCount;
}DB_CTX, *PDB_CTX;

#endif /* db_h */

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

typedef struct _DB_INDEX_MEMBER {
    WORD64 IndexValue;
    WORD64 EntryOffset;
}DB_INDEX_MEMBER, *PDB_INDEX_MEMBER;

typedef struct _DB_INDEX {
    WORD64 Previous, Me, Next;
    
    WORD64 FirstMemberOffset;
    WORD64 MemberSize;
}DB_INDEX, *PDB_INDEX;

typedef struct _DB_ENTRY {
    WORD64 Previous, Me, Next;
    
    WORD64 DataPtr;
    WORD64 QuickIndexValue;
}DB_ENTRY, *PDB_ENTRY;

typedef struct _DB_TABLE {
    WORD64 Previous, Me, Next;
    
    char Name[128];
    WORD64 FirstEntryOffset;
    WORD64 FirstIndexOffset;
}DB_TABLE, *PDB_TABLE;

typedef struct _DB_BASE {
    WORD64 FirstTableOffset;
}DB_BASE, *PDB_BASE;


#endif /* db_h */

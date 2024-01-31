//
//  db.h
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#ifndef db_h
#define db_h

#include "../types.h"
#include <time.h>
#include <stdio.h>

/*
 Certain structures contain data does not
 need to be saved, but is useful to store
 locally in the structure. These are at the
 end of the structure, and the SZ__* sizes
 represents the truncated structure.
 */

typedef struct _DB_ENTRY_INDEX_MEMBER {
    WORD64 Iterator;
    WORD64 EntryOffset;
    WORD64 Index0, Index1;
}DB_ENTRY_INDEX_MEMBER, *PDB_ENTRY_INDEX_MEMBER;
#define SZ__DB_ENTRY_INDEX_MEMBER sizeof(DB_ENTRY_INDEX_MEMBER)

typedef struct _DB_ENTRY_INDEX {
    WORD64 IndexMemberSize;
    WORD64 FirstIndexMember;
    
    WORD64 Previous, Next;
    PDB_ENTRY_INDEX_MEMBER IndexMembers;
}DB_ENTRY_INDEX, *PDB_ENTRY_INDEX;
#define SZ__DB_ENTRY_INDEX sizeof(DB_ENTRY_INDEX) - sizeof(PDB_ENTRY_INDEX_MEMBER)

typedef union _DB_ENTRY_FIELD_DATA {
    void* Buffer;
    char* Buffer_String;
    WORD64* Buffer_ULong64;
    WORD32* Buffer_ULong32;
}DB_ENTRY_FIELD_DATA, *PDB_ENTRY_FIELD_DATA;

typedef struct _DB_ENTRY_FIELD {
    WORD64 EntryId;
    WORD64 SubEntryId;
    WORD64 FieldId;
    
    WORD64 DataOffset;
    WORD64 DataSize;
    
    DB_ENTRY_FIELD_DATA ResolvedData;
}DB_ENTRY_FIELD, *PDB_ENTRY_FIELD;
#define SZ__DB_ENTRY_FIELD sizeof(DB_ENTRY_FIELD) - sizeof(DB_ENTRY_FIELD_DATA)

typedef struct _DB_ENTRY {
    WORD64 EntryId;
    WORD64 ParentTable;
    WORD64 Previous, Next;
    time_t Created;
    time_t Modified;
    
    WORD64 FirstEntryField;
    WORD64 EntryFieldCount;
    PDB_ENTRY_FIELD EntryFields;
}DB_ENTRY, *PDB_ENTRY;
#define SZ__DB_ENTRY sizeof(DB_ENTRY) - sizeof(PDB_ENTRY_FIELD)

typedef struct _DB_FIELD {
    WORD64 FieldId;
    WORD64 ParentTable;
    WORD64 Previous, Next;
    time_t Created;
    time_t Modified;
    
    char FieldName[128];
    int FieldType;
}DB_FIELD, *PDB_FIELD;
#define SZ__DB_FIELD sizeof(DB_FIELD)

#define DB_FIELDTYPE_STRING  0x00
#define DB_FIELDTYPE_ULONG64 0x01
#define DB_FIELDTYPE_ULONG32 0x02

typedef struct _DB_TABLE {
    WORD64 TableId;
    char TableName[128];
    time_t Created;
    time_t LastModified;
    WORD64 Previous, Next;
    
    WORD64 FirstFieldOffset;
    WORD64 FieldCount;
    
    WORD64 FirstEntryOffset;
    WORD64 EntryCount;
    
    WORD64 FirstEntryIndex;
    WORD64 EntryIndexField0;
    WORD64 EntryIndexField1;
    WORD64 EntryIndexCount;
    
    PDB_ENTRY_INDEX EntryIndices;
    PDB_ENTRY Entries;
    PDB_FIELD Fields;
}DB_TABLE, *PDB_TABLE;
#define SZ__DB_TABLE sizeof(DB_TABLE) - (sizeof(void*) * 3)

typedef struct _DB_BASE {
    WORD64 LocalBaseId;
    time_t Created;
    time_t Modified;
    
    WORD64 FirstTableOffset;
    WORD64 IteratorBase;
    
    PDB_TABLE Tables; // not req. save
    WORD64 TableCount; // not req. save
    FILE* FilePointer; // not req. save
}DB_BASE, *PDB_BASE;
#define SZ__DB_BASE sizeof(DB_BASE) - (sizeof(PDB_TABLE) + sizeof(WORD64) + sizeof(FILE*))

typedef struct _DB_CTX {
    PDB_BASE Bases;
    WORD64 BaseCount;
    
    WORD64 IteratorBase;
    WORD64 ActiveWorkingBase;
}DB_CTX, *PDB_CTX;
extern PDB_CTX DbCtx;

void DbInit(void);
void DbShutdown(void);

WORD64 DbCreateBase(const char* FileName);
WORD64 DbOpenBase(const char* FileName);
void   DbCloseBase(WORD64 BaseId);
void DbSetWorkingBase(WORD64 BaseId);

WORD64 DbBaseCreateTable(const char* TableName);
void   DbBaseDeleteTable(WORD64 TableId);
void   DbBaseCreateField(WORD64 TableId, const char* Name, int Type);
WORD64 DbBaseDeleteField(WORD64 TableId, WORD64 FieldId);
WORD64 DbBaseGetFieldByName(WORD64 TableId, const char* Name);

WORD64 DbBaseCreateEntry(WORD64 TableId);
void   DbBaseAddToEntryString(WORD64 TableId, WORD64 FieldId,
    const char* Data);
void   DbBaseAddToEntryULong64(WORD64 TableId, WORD64 FieldId,
    WORD64 Data);
void   DbBaseAddToEntryULong32(WORD64 TableId, WORD64 FieldId,
    WORD32 Data);

#endif /* db_h */

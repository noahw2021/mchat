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

typedef struct _DB_ENTRY_FIELD {
    WORD64 EntryId;
    WORD64 SubEntryId;
    WORD64 FieldId;
    
    WORD64 DataOffset;
    WORD64 DataSize;
}DB_ENTRY_FIELD, *PDB_ENTRY_FIELD;

typedef struct _DB_ENTRY {
    WORD64 EntryId;
    WORD64 ParentTable;
    WORD64 Previous, Next;
    time_t Created;
    time_t Modified;
    
    WORD64 FirstEntryField;
    PDB_ENTRY_FIELD EntryFields;
    WORD64 EntryFieldCount;
}DB_ENTRY, *PDB_ENTRY;

typedef struct _DB_FIELD {
    WORD64 FieldId;
    WORD64 ParentTable;
    WORD64 Previous, Next;
    time_t Created;
    time_t Modified;
    
    char FieldName[128];
    int FieldType;
}DB_FIELD, *PDB_FIELD;

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
    PDB_FIELD Fields;
    WORD64 FieldCount;
    
    WORD64 FirstEntryOffset;
    PDB_ENTRY Entries;
    WORD64 EntryCount;
}DB_TABLE, *PDB_TABLE;

typedef struct _DB_BASE {
    time_t Created;
    time_t Modified;
    
    WORD64 FirstTableOffset;
    PDB_TABLE Tables;
    WORD64 TableCount;
    
    FILE* FilePointer;
    WORD64 IteratorBase;
}DB_BASE, *PDB_BASE;

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

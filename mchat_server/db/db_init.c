//
//  db_init.c
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#include "db.h"
#include <stdlib.h>
#include <string.h>

PDB_CTX DbCtx;

void DbInit(void) {
    DbCtx = malloc(sizeof(DB_CTX));
    memset(DbCtx, 0, sizeof(DB_CTX));
    
    return;
}

void DbShutdown(void) {
    if (!DbCtx)
        return;
    
    if (DbCtx->Bases) {
        for (int i = 0; i < DbCtx->BaseCount; i++) {
            PDB_BASE ThisBase = &DbCtx->Bases[i];
            if (ThisBase->Tables) {
                for (int t = 0; t < ThisBase->TableCount; t++) {
                    PDB_TABLE ThisTable = &ThisBase->Tables[t];
                    
                    if (ThisTable->Entries) {
                        for (int e = 0; e < ThisTable->EntryCount; e++) {
                            PDB_ENTRY ThisEntry = &ThisTable->Entries[e];
                            
                            if (ThisEntry->EntryFields) {
                                for (int ef = 0;
                                    ef <ThisEntry->EntryFieldCount; ef++
                                ) {
                                    PDB_ENTRY_FIELD ThisEntryField =
                                        &ThisEntry->EntryFields[ef];
                                    
                                    if (ThisEntryField->ResolvedData
                                        .Buffer
                                    ) {
                                        free(ThisEntryField->
                                            ResolvedData.Buffer);
                                    }
                                }
                                
                                free(ThisEntry->EntryFields);
                            }
                        }
                        
                        free(ThisTable->Entries);
                    }
                    
                    if (ThisTable->EntryIndices) {
                        for (int ei = 0; ei < ThisTable->EntryIndexCount;
                             ei++
                        ) {
                            
                        }
                    }
                    
                    if (ThisTable->Fields)
                        free(ThisTable->Fields);
                    
                }
                
                free(ThisBase->Tables);
            }
        }
        
        free(DbCtx->Bases);
    }
}

void DbSetWorkingBase(WORD64 BaseId) {
    if (!DbCtx)
        return;
    
    DbCtx->ActiveWorkingBase = BaseId;
    return;
}

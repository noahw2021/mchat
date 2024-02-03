//
//  db_init.c
//  mchat_server
//
//  Created by Noah Wooten on 2/3/24.
//

#include "db.h"
PDB_CTX DbCtx;

#include <stdlib.h>
#include <string.h>

void DbInit(void) {
    DbCtx = malloc(sizeof(DB_CTX));
    memset(DbCtx, 0, sizeof(DB_CTX));
    
    return;
}

void DbShutdown(void) {
    if (!DbCtx)
        return;
    
    if (DbCtx->Bases) {
        for (int b = 0; b < DbCtx->BaseCount; b++) {
            PDB_BASE ThisBase = DbCtx->Bases[b];
            
            for (int t = 0; t < ThisBase->TableCount; t++) {
                PDB_TABLE ThisTable = ThisBase->Tables[t];
                
                for (int e = 0; e < ThisTable->EntryCount; e++) {
                    PDB_ENTRY ThisEntry = ThisTable->Entries[e];
                    
                    if (ThisEntry->EntryValue)
                        free(ThisEntry->EntryValue);
                    
                    free(ThisEntry);
                }
                
                if (ThisTable->Entries)
                    free(ThisTable->Entries);
                
                for (int i = 0; i < ThisTable->IndexCount; i++) {
                    PDB_INDEX ThisIndex = ThisTable->Indices[i];
                    
                    if (ThisIndex->IndexMembers)
                        free(ThisIndex->IndexMembers);
                    
                    free(ThisIndex);
                }
                
                if (ThisTable->Indices)
                    free(ThisTable->Indices);
                
                free(ThisTable);
            }
            
            free(ThisBase);
        }
    
        free(DbCtx->Bases);
    }
}


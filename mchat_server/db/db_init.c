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
    
    pthread_mutex_init(&DbCtx->BasesMutex, NULL);
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
                    
                    pthread_mutex_destroy(&ThisEntry->EntryMutex);
                    free(ThisEntry);
                }
                
                if (ThisTable->Entries)
                    free(ThisTable->Entries);
                
                for (int i = 0; i < ThisTable->IndexCount; i++) {
                    PDB_INDEX ThisIndex = ThisTable->Indices[i];
                    
                    if (ThisIndex->IndexMembers)
                        free(ThisIndex->IndexMembers);
                    
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
    
        free(DbCtx->Bases);
    }
}


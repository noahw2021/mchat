//
//  db.h
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#ifndef db_h
#define db_h

void DbInit(void);
void DbShutdown(void);
void DbLock(void);
void DbUnlock(void);

typedef struct _DBSTRUCT_USER {
    
}DBSTRUCT_USER, *PDBSTRUCT_USER;

typedef struct _DBSTRUCT_CHANNEL {
    
}DBSTRUCT_CHANNEL, *PDBSTRUCT_CHANNEL;

typedef struct _DBSTRUCT_MESSAGE {
    
}DBSTRUCT_MESSAGE, *PDBSTRUCT_MESSAGE;

typedef struct _DBSTRUCT {
    
}DBSTRUCT, *PDBSTRUCT;

typedef struct _DBCTX {
    
}DBCTX, *PDBCTX;

#endif /* db_h */

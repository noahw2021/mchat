//
//  db.h
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#ifndef db_h
#define db_h

#include "../../types.h"

void DbInit(void);
void DbShutdown(void);
void DbLock(void);
void DbUnlock(void);

typedef struct _DBSTRUCT_USER {
    wchar_t Username[128];
    WORD64 UserID[2];
    WORD64 DirectMessageChannel[2];
    
    // Linked list stuff
    WORD32 Previous;
    WORD32 Me;
    WORD32 Next;
}DBSTRUCT_USER, *PDBSTRUCT_USER;

typedef struct _DBSTRUCT_CHANNEL {
    WORD64 ChannelID[2];
    WORD64 ChannelCryptor[4];
    wchar_t ChannelName[128];
    WORD64 LastMessage[2];
    WORD64 MessageCount;
    
    // Linked list stuff
    WORD32 Previous;
    WORD32 Me;
    WORD32 Next;
}DBSTRUCT_CHANNEL, *PDBSTRUCT_CHANNEL;

typedef struct _DBSTRUCT_MESSAGE {
    WORD64 MessageID[2];
    WORD64 ChannelID[2];
    wchar_t Message[2048];
    time_t Sent;
    
    // Linked list stuff
    WORD32 Previous;
    WORD32 Me;
    WORD32 Next;
}DBSTRUCT_MESSAGE, *PDBSTRUCT_MESSAGE;

typedef struct _DBSTRUCT {
    WORD32 FirstMessage;
    WORD32 FirstChannel;
    WORD32 FirstUser;
    
    WORD32 MessageCount;
    WORD32 ChannelCount;
    WORD32 UserCount;
    
    PDBSTRUCT_MESSAGE Messages;
    PDBSTRUCT_CHANNEL Channels;
    PDBSTRUCT_USER Users;
}DBSTRUCT, *PDBSTRUCT;

typedef struct _DBCTX {
    void* DbMutex;
    FILE* DbFile;
    PDBSTRUCT DbStruct;
}DBCTX, *PDBCTX;
extern PDBCTX DbCtx;

PDBSTRUCT_USER       DbGetUserByID(WORD64* ID);
PDBSTRUCT_CHANNEL DbGetChannelByID(WORD64* ID);
PDBSTRUCT_MESSAGE DbGetMessageByID(WORD64* ID);
PDBSTRUCT_USER       DbGetUserByIterator(WORD64* ID);
PDBSTRUCT_CHANNEL DbGetChannelByIterator(WORD64* ID);
PDBSTRUCT_MESSAGE DbGetMessageByIterator(WORD64* ID);
PDBSTRUCT_USER    DbGetUserByName(wchar_t* Name);

void DbUpdateUser(PDBSTRUCT_USER User);
void DbUpdateChannel(PDBSTRUCT_CHANNEL Channel);
void DbUpdateMessage(PDBSTRUCT_MESSAGE Message);

#endif /* db_h */

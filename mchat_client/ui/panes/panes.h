//
//  panes.h
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#ifndef panes_h
#define panes_h

// Chat context structures
#include "../../chat/chatp/chatp.h"
#include <time.h>
#include <pthread.h>

typedef struct _UI_MESSAGE {
    wchar_t* Author;
    wchar_t* Body;
    time_t TimeSent;
}UI_MESSAGE, *PUI_MESSAGE;

typedef struct _UIPCTX {
    int CurrentRenderingPane;
    PCHATP_CHANNEL ActiveChannel;
    PCHATP_MESSAGE EditingMessage;
    wchar_t MyUsername[128];
    
    PUI_MESSAGE Messages;
    WORD32 MessageCount;
    pthread_mutex_t MessageMutex;
}UIPCTX, *PUIPCTX;
extern PUIPCTX UipCtx;

void UipInit(void);
void UipShutdown(void);
void UipBeginScene(void);
void UipEndScene(void);

void UipRenderChannel(PCHATP_CHANNEL Channel);
void UipRenderUserInfo(PCHATP_CHANNEL Channel);
void UipRenderRequests(PCHATP_REQUEST Requests, WORD32 Count);

void UispInputHandler(void);
void UispRenderNewMessage(wchar_t* Author, wchar_t* Message,
    time_t SendDate);

#endif /* panes_h */

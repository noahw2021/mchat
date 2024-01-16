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

typedef struct _UIPCTX {
    int CurrentRenderingPane;
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
void UispMessageRenderer(PCHATP_MESSAGE Message);
void UispRenderNewMessage(wchar_t* Author, wchar_t* Message,
    time_t SendDate);

#endif /* panes_h */

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

void UispInputHandler(void);
void UispMessageRenderer(PCHATP_MESSAGE Message);

#endif /* panes_h */

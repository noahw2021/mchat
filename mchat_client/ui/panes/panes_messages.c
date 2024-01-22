//
//  panes_messages.c
//  mchat_client
//
//  Created by Noah Wooten on 1/22/24.
//

#include "panes.h"
#include <stdlib.h>
#include <string.h>

void UispRenderNewMessage(wchar_t* Author, wchar_t* Message,
    time_t SendDate
){
    if (!UipCtx->Messages) {
        UipCtx->Messages = malloc(sizeof(UI_MESSAGE));
    } else {
        UipCtx->Messages = realloc(UipCtx->Messages,
            sizeof(UI_MESSAGE) * (UipCtx->MessageCount + 1));
    }
    
    PUI_MESSAGE NewMsg = &UipCtx->Messages[UipCtx->MessageCount];
    memset(NewMsg, 0, sizeof(UI_MESSAGE));
    UipCtx->MessageCount++;
    
    NewMsg->Author = malloc(128 * sizeof(wchar_t));
    NewMsg->Body = malloc(2048 * sizeof(wchar_t));
    NewMsg->TimeSent = SendDate;
    wcscpy(NewMsg->Author, Author);
    wcscpy(NewMsg->Body, Message);
    
    return;
}

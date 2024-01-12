//
//  ui_render.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "ui.h"
#include "panes/panes.h"

#include "../chat/chatp/chatp.h"

void UiRender(void) {
    // get the size of the window
    int w, h;
    UiiGetSize(&w, &h);
    
    // render the input bar group
    pthread_mutex_lock(&UiCtx->AccessMutex);
    int InputHeight = (2048 / w) + 1;
    UiCtx->InputBufferHeight = InputHeight;
    pthread_mutex_unlock(&UiCtx->AccessMutex);
    UiiSetPos(0, InputHeight);
    // draw white on top of the input bar
    UiiPushColor(UIICOLOR_WHITEBG);
    for (int i = 0; i < w; i++)
        fputc(' ', stdout);
    UiiPopColor();
    
    // render the active pane
    pthread_mutex_lock(&UiCtx->AccessMutex);
    int ThisPane = UiCtx->CurrentPane;
    pthread_mutex_unlock(&UiCtx->AccessMutex);
    
    switch (ThisPane) {
        case PANE_CHANNEL:
            pthread_mutex_lock(&UiCtx->AccessMutex);
            PCHATP_CHANNEL ThisChannel = UiCtx->CurrentChannel;
            pthread_mutex_unlock(&UiCtx->AccessMutex);
            UipRenderChannel(ThisChannel);
            break;
        case PANE_REQUESTS:
            UipRenderRequests(ChatpCtx->Requests, 
                ChatpCtx->RequestCount);
            break;
        case PANE_USERINFO:
            pthread_mutex_lock(&UiCtx->AccessMutex);
            PCHATP_CHANNEL ThisChannel2 = UiCtx->CurrentChannel;
            pthread_mutex_unlock(&UiCtx->AccessMutex);
            UipRenderUserInfo(ThisChannel2);
            break;
        default:
            pthread_mutex_lock(&UiCtx->AccessMutex);
            UiCtx->CurrentPane = PANE_CHANNEL;
            pthread_mutex_unlock(&UiCtx->AccessMutex);
            break;
    }
}

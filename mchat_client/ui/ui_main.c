//
//  ui_main.c
//  mchat_client
//
//  Created by Noah Wooten on 1/10/24.
//

#include "ui.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../chat/chat.h"
#include "../chat/chatp/chatp.h"

PUI_CTX UiCtx;

void* UiiThread(void* Reserved) {
    WORD64 Ticks = 0;
    while (1) {
        // check if we are shutting down
        pthread_mutex_lock(&UiCtx->AccessMutex);
        if (UiCtx->WantsShutdown) {
            pthread_mutex_unlock(&UiCtx->AccessMutex);
            break;
        }
        pthread_mutex_unlock(&UiCtx->AccessMutex);
        
        usleep(1000000); // sleep for 1s
        Ticks++; // increment tick count
        
        if ((Ticks % 10) == 0) { // every 10 seconds
            // do something later
        }
        
        ChatpUpdateContext(); // update the chat context
        UiRender(); // update the user interface
    }
    
    return NULL;
}

void UiMain(void) {
    UiiClear();
    
    UiCtx = malloc(sizeof(UI_CTX));
    memset(UiCtx, 0, sizeof(UI_CTX));
    pthread_mutex_init(&UiCtx->AccessMutex, NULL);
    
    return;
}


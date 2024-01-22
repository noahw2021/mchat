//
//  panes_init.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "panes.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

PUIPCTX UipCtx;

void UipInit(void) {
    UipCtx = malloc(sizeof(UIPCTX));
    memset(UipCtx, 0, sizeof(UIPCTX));
    
    pthread_mutex_init(&UipCtx->MessageMutex, NULL);
    return;
}

void UipShutdown(void) {
    if (!UipCtx)
        return;
    
    if (UipCtx->Messages)
        free(UipCtx->Messages);
    
    pthread_mutex_destroy(&UipCtx->MessageMutex);
    free(UipCtx);
    return;
}

void UipBeginScene(void) {
    UipRenderChannel(UipCtx->ActiveChannel);
    return;
}

void UipEndScene(void) {
    return;
}

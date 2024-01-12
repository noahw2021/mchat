//
//  panes_init.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "panes.h"
#include <stdlib.h>
#include <string.h>

PUIPCTX UipCtx;

void UipInit(void) {
    UipCtx = malloc(sizeof(UIPCTX));
    memset(UipCtx, 0, sizeof(UIPCTX));
    
    return;
}

void UipShutdown(void) {
    free(UipCtx);
    return;
}

void UipBeginScene(void) {
    return;
}

void UipEndScene(void) {
    return;
}

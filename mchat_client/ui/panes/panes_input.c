//
//  panes_input.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "panes.h"
#include "../ui.h"

void UispInputHandler(void) {
    
    // set the current line position
    pthread_mutex_lock(&UiCtx->AccessMutex);
    int LineHeight = UiCtx->InputBufferHeight;
    pthread_mutex_unlock(&UiCtx->AccessMutex);
    UiiSetPos(0, LineHeight);
    
    // query for input
    wchar_t InputBuffer[2048];
    fgetws(InputBuffer, 2048, stdin);
    
    // re-acquire the mutex, copy the buffer
    pthread_mutex_lock(&UiCtx->AccessMutex);
    wcslcpy(UiCtx->InputBuffer, InputBuffer, 2048);
    pthread_mutex_unlock(&UiCtx->AccessMutex);
    
    if (*InputBuffer == L':') {
        // treat this as a command
        wchar_t* Command = InputBuffer + 1;
        
    }
}

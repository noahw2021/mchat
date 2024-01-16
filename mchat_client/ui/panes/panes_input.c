//
//  panes_input.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "panes.h"
#include "../ui.h"
#include "../../chat/chat.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
        if (!wcscmp(Command, L"h")) {
            UispRenderNewMessage(L"System", L":h -- Displays this.",
                time(NULL));
            UispRenderNewMessage(L"System", L":v -- View friends list.",
                time(NULL) + 1);
            UispRenderNewMessage(L"System", L":a -- View request list.",
                time(NULL) + 2);
            UispRenderNewMessage(L"System", L":r -- Reject request.",
                time(NULL) + 3);
            UispRenderNewMessage(L"System", L":y -- Accept request.",
                time(NULL) + 4);
            UispRenderNewMessage(L"System", L":t -- Create request.",
                time(NULL) + 5);
            UispRenderNewMessage(L"System", L":d -- Delete message.",
                time(NULL) + 6);
            UispRenderNewMessage(L"System", L":t -- Edit message.",
                time(NULL) + 7);
            
        } else if (!wcscmp(Command, L"v")) {
            pthread_mutex_lock(&UiCtx->AccessMutex);
            UiCtx->CurrentPane = PANE_FRIENDS;
            pthread_mutex_unlock(&UiCtx->AccessMutex);
        } else if (!wcscmp(Command, L"a")) {
            pthread_mutex_lock(&UiCtx->AccessMutex);
            UiCtx->CurrentPane = PANE_REQUESTS;
            pthread_mutex_unlock(&UiCtx->AccessMutex);
        } else if (!wcscmp(Command, L"r")) {
            long ID = wcstol(Command + 2, NULL, 10);
            if (ID >= ChatpCtx->RequestCount) {
                UispRenderNewMessage(L"System", L"Invalid user.",
                    time(NULL));
            } else {
                PCHATP_REQUEST Request = &ChatpCtx->Requests[ID - 1];
                PCHAT_YES NetReq = malloc(sizeof(CHAT_YES));
                memset(NetReq, 0, sizeof(CHAT_YES));
                memcpy(NetReq->ReceipID, ChatCtx->MyID, 
                    sizeof(WORD64) * 2);
                memcpy(NetReq->SenderID, Request->TheirID,
                       sizeof(WORD64) * 2);
                
                ChatEventSendYes(NetReq);
                free(NetReq);
            }
        }
    }
}

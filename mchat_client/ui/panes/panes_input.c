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
#include <unistd.h>
#include <wchar.h>

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
        if (!wcscmp(Command, L"h")) { // Help Command
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
            UispRenderNewMessage(L"System", L":e -- Edit message.",
                time(NULL) + 7);
            UispRenderNewMessage(L"System", L":c -- Cancel edit.",
                time(NULL) + 7);
            
        } else if (!wcscmp(Command, L"v")) { // View Friends Command
            pthread_mutex_lock(&UiCtx->AccessMutex);
            UiCtx->CurrentPane = PANE_FRIENDS;
            pthread_mutex_unlock(&UiCtx->AccessMutex);
        } else if (!wcscmp(Command, L"a")) { // View Requests Command
            pthread_mutex_lock(&UiCtx->AccessMutex);
            UiCtx->CurrentPane = PANE_REQUESTS;
            pthread_mutex_unlock(&UiCtx->AccessMutex);
        } else if (!wcscmp(Command, L"r")) { // Reject Request
            long ID = wcstol(Command + 2, NULL, 10);
            if (ID >= ChatpCtx->RequestCount) {
                UispRenderNewMessage(L"System", L"Invalid user.",
                    time(NULL));
            } else {
                PCHATP_REQUEST Request = &ChatpCtx->Requests[ID - 1];
                PCHAT_REJECT NetReq = malloc(sizeof(CHAT_REJECT));
                memset(NetReq, 0, sizeof(CHAT_YES));
                memcpy(NetReq->MyID, ChatCtx->MyID,
                    sizeof(WORD64) * 2);
                memcpy(NetReq->TheirID, Request->TheirID,
                       sizeof(WORD64) * 2);
                
                ChatEventSendReject(NetReq);
                free(NetReq);
            }
        } else if (!wcscmp(Command, L"y")) { // Accept Request
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
        } else if (!wcscmp(Command, L"t")) { // Create request
            PCHAT_GETUSERNAME Req = malloc(sizeof(CHAT_GETUSERNAME));
            memset(Req, 0, sizeof(CHAT_GETUSERNAME));
            
            memcpy(Req->MyID, ChatCtx->MyID, sizeof(WORD64) * 4);
            wcslcpy(Req->MyUsername, Command + 2, 64);
            ChatpCtx->UsernameFlag = 0;
            
            ChatEventSendGetUsername(Req);
            free(Req);
            while (!ChatpCtx->UsernameFlag)
                usleep(100000);
            
            PCHAT_HELLO Req2 = malloc(sizeof(CHAT_HELLO));
            memset(Req2, 0, sizeof(CHAT_HELLO));
            
            memcpy(Req2->ReceipID, ChatpCtx->LastRecvID,
                sizeof(WORD64) * 4);
            wcslcpy(Req2->ReptUsername,
                ChatpCtx->LastReturnedUsername, 64);
            wcslcpy(Req2->SendUsername, ChatCtx->MyUsername, 64);
            memcpy(Req2->SenderID, ChatCtx->MyID, sizeof(WORD64) * 4);
            
            ChatEventSendHello(Req2);
            free(Req2);
        } else if (!wcscmp(Command, L"d")) {
            int Counter = 0;
            long Argument = wcstol(Command + 2, NULL, 10);
            for (int i = 0; i < UipCtx->ActiveChannel->MessageCount; 
                 i++
            ) {
                PCHATP_MESSAGE ThisMsg = ChatpCtx->Messages[i];
                if (ThisMsg->ChannelID[0] !=
                    UipCtx->ActiveChannel->ChannelID[0] ||
                    ThisMsg->ChannelID[1] !=
                    UipCtx->ActiveChannel->ChannelID[1]
                ) {
                    continue;
                }
                
                Counter++;
                if (Counter == Argument) {
                    ThisMsg->WasMessageDeleted = 1;
                    PCHAT_MESSAGEUPDATE MsgUpdate = 
                        malloc(sizeof(CHAT_MESSAGEUPDATE));
                    memset(MsgUpdate, 0, sizeof(CHAT_MESSAGEUPDATE));
                    
                    memcpy(MsgUpdate->MessageID, ThisMsg->MessageId, 
                        sizeof(WORD64) * 2);
                    MsgUpdate->WasDeleted = 1;
                    
                    ChatEventSendMessageUpdate(MsgUpdate);
                    free(MsgUpdate);
                    return;
                }
            }
        } else if (!wcscmp(Command, L"e")) {
            int Counter = 0;
            long Argument = wcstol(Command + 2, NULL, 10);
            for (int i = 0; i < UipCtx->ActiveChannel->MessageCount;
                 i++
            ) {
                PCHATP_MESSAGE ThisMsg = ChatpCtx->Messages[i];
                if (ThisMsg->ChannelID[0] !=
                    UipCtx->ActiveChannel->ChannelID[0] ||
                    ThisMsg->ChannelID[1] !=
                    UipCtx->ActiveChannel->ChannelID[1]
                    ) {
                    continue;
                }
                
                Counter++;
                if (Counter == Argument) {
                    UipCtx->EditingMessage = ThisMsg;
                    wchar_t* EditMsg = malloc(sizeof(wchar_t) * 128);
                    
                    swprintf(EditMsg, 128,
                        L"Edit your message (%i), or ':c' to cancel.",
                        Argument);
                    
                    UispRenderNewMessage(L"System", EditMsg, 
                        time(NULL));
                    free(EditMsg);
                    return;
                }
            }
        } else if (!wcscmp(Command, L"c")) { // Cancel edit
            if (UipCtx->EditingMessage) {
                UispRenderNewMessage(L"System", L"Edit cancelled.",
                    time(NULL));
                return;
            }
            
            UipCtx->EditingMessage = NULL;
        } else { // invalid command
            UispRenderNewMessage(L"System", L"Invalid command.",
                time(NULL));
            return;
        }
    }
}

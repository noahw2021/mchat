//
//  chat_sendevents.c
//  mchat_client
//
//  Created by Noah Wooten on 1/10/24.
//

#include "chat.h"
#include <stdlib.h>
#include <stdio.h>
#include "../netw/netw.h"

void ChatEventSendRegister(PCHAT_REGISTER Event) {
    char* Request = malloc(256);
    sprintf(Request, "api0/Events_SendEvent_Register?Secret="
        "%016llX%016llX%016llX%016llX", ChatCtx->MySecret[0],
        ChatCtx->MySecret[1], ChatCtx->MySecret[2], ChatCtx->MySecret[3]);
    
    char* B64Buffer = malloc(CHATSZ_MAX * 10);
    NetwEncrypt256(Event, sizeof(CHAT_REGISTER), ChatCtx->ClientKey);
    NetwBase64Encode(B64Buffer, CHATSZ_MAX * 10, Event, sizeof(CHAT_REGISTER));
    
    NetwPOSTApi(Request, B64Buffer);
    free(B64Buffer);
    
    return;
}

void ChatEventSendHello(PCHAT_HELLO Event) {
    char* Request = malloc(256);
    sprintf(Request, "api0/Events_SendEvent_Hello?Secret="
        "%016llX%016llX%016llX%016llX", ChatCtx->MySecret[0],
        ChatCtx->MySecret[1], ChatCtx->MySecret[2], ChatCtx->MySecret[3]);
    
    char* B64Buffer = malloc(CHATSZ_MAX * 10);
    NetwEncrypt256(Event, sizeof(CHAT_HELLO), ChatCtx->ClientKey);
    NetwBase64Encode(B64Buffer, CHATSZ_MAX * 10, Event, sizeof(CHAT_HELLO));
    
    NetwPOSTApi(Request, B64Buffer);
    free(B64Buffer);
    
    return;
}

void ChatEventSendGetUsername(PCHAT_GETUSERNAME Event) {
    char* Request = malloc(256);
    sprintf(Request, "api0/Events_SendEvent_GetUsername?Secret="
        "%016llX%016llX%016llX%016llX", ChatCtx->MySecret[0],
        ChatCtx->MySecret[1], ChatCtx->MySecret[2], ChatCtx->MySecret[3]);
    
    char* B64Buffer = malloc(CHATSZ_MAX * 10);
    NetwEncrypt256(Event, sizeof(CHAT_GETUSERNAME), ChatCtx->ClientKey);
    NetwBase64Encode(B64Buffer, CHATSZ_MAX * 10, Event, sizeof(CHAT_GETUSERNAME));
    
    NetwPOSTApi(Request, B64Buffer);
    free(B64Buffer);
    
    return;
}

void ChatEventSendMessage(PCHAT_MESSAGE Event) {
    char* Request = malloc(256);
    sprintf(Request, "api0/Events_SendEvent_Message?Secret="
        "%016llX%016llX%016llX%016llX", ChatCtx->MySecret[0],
        ChatCtx->MySecret[1], ChatCtx->MySecret[2], ChatCtx->MySecret[3]);
    
    char* B64Buffer = malloc(CHATSZ_MAX * 10);
    NetwEncrypt256(Event, sizeof(CHAT_MESSAGE), ChatCtx->ClientKey);
    NetwBase64Encode(B64Buffer, CHATSZ_MAX * 10, Event, sizeof(CHAT_MESSAGE));
    
    NetwPOSTApi(Request, B64Buffer);
    free(B64Buffer);
    
    return;
}

void ChatEventSendLogin(PCHAT_LOGIN Event) {
    char* Request = malloc(256);
    sprintf(Request, "api0/Events_SendEvent_Login?Secret="
        "%016llX%016llX%016llX%016llX", ChatCtx->MySecret[0],
        ChatCtx->MySecret[1], ChatCtx->MySecret[2], ChatCtx->MySecret[3]);
    
    char* B64Buffer = malloc(CHATSZ_MAX * 10);
    NetwEncrypt256(Event, sizeof(CHAT_LOGIN), ChatCtx->ClientKey);
    NetwBase64Encode(B64Buffer, CHATSZ_MAX * 10, Event, sizeof(CHAT_LOGIN));
    
    NetwPOSTApi(Request, B64Buffer);
    free(B64Buffer);
    
    return;
}

//
//  chat_recvevents.c
//  mchat_client
//
//  Created by Noah Wooten on 1/9/24.
//

#include "chat.h"
#include "../netw/netw.h"

#include <stdlib.h>
#include <string.h>

int ChatGetEventList(void) {
    char* Request = malloc(256);
    sprintf(Request, "api0/Events_GetList?Secret="
        "%016llX%016llX%016llX%016llX&ClientFingerprint=%016llX%016llX",
        ChatCtx->MySecret[0], ChatCtx->MySecret[1], ChatCtx->MySecret[2],
        ChatCtx->MySecret[3], ChatCtx->ClientFingerprint[0],
        ChatCtx->ClientFingerprint[1]);
    
    char* Response = NetwGETApi(Request);
    free(Request);
    
    PCHAT_LIST List = malloc(sizeof(CHAT_LIST));
    NetwBase64Decode(List, sizeof(CHAT_LIST), Response);
    free(Response);
    NetwDecrypt256(List, sizeof(CHAT_LIST), ChatCtx->ClientKey);
    
    if (ChatCtx->PreviousList)
        free(ChatCtx->PreviousList);
    
    ChatCtx->PreviousList = List;
    
    return List->EventCount;
}

int ChatGetEventType(int i) {
    if (!ChatCtx->PreviousList)
        return -1;
    
    return ChatCtx->PreviousList->EventTypes[i];
}

void* ChatiGetEvent(int i) {
    char* Request = malloc(256);
    sprintf(Request, "api0/Events_GetObject?Secret="
        "%016llX%016llX%016llX%016llX&EventId=%i"
        "&ClientFingerprint=%016llX%016llX", ChatCtx->MySecret[0],
        ChatCtx->MySecret[1], ChatCtx->MySecret[2], ChatCtx->MySecret[3],
        i, ChatCtx->ClientFingerprint[0], ChatCtx->ClientFingerprint[1]);
    
    char* Response = NetwGETApi(Request);
    free(Request);
    
    void* Buffer = malloc(CHATSZ_MAX);
    NetwBase64Decode(Buffer, CHATSZ_MAX, Response);
    free(Response);
    NetwDecrypt256(Buffer, CHATSZ_MAX, ChatCtx->ClientKey);
    
    return Buffer;
}

PCHAT_YES ChatGetEventAsYes(int i) {
    return ChatiGetEvent(i);
}

PCHAT_MESSAGE ChatGetEventAsMessage(int i) {
    return ChatiGetEvent(i);
}


PCHAT_RECVUSERNAME ChatGetEventAsRecvUsername(int i) {
    return ChatiGetEvent(i);
}

PCHAT_LOGINRECV ChatGetEventAsRecvLogin(int i) {
    return ChatiGetEvent(i);
}


PCHAT_MESSAGEDELETERECV ChatGetEventAsRecvMessageDelete(int i){
    return ChatiGetEvent(i);
}

PCHAT_MESSAGEUPDATERECV ChatGetEventAsRecvMessageUpdate(int i) {
    return ChatiGetEvent(i);
}

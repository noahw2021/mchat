//
//  chatp_context.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "chatp.h"
#include "../chat.h"
#include "../db/db.h"

#include <stdlib.h>
#include <string.h>

void ChatpUpdateContext(void) {
    int UnreadEvents = ChatGetEventList();
    
    for (int i = 0; i < UnreadEvents; i++) {
        int ThisEventType = ChatGetEventTypes(i);
        void* _Event = ChatiGetEvent(i);
        
        switch (ThisEventType) {
            case CHATEVENTTYPE_YES:
                ChatpHandler_Yes(_Event);
                break;
            case CHATEVENTTYPE_RECVLOGIN:
                ChatpHandler_RecvLogin(_Event);
                break;
            case CHATEVENTTYPE_RECVUSERNAME:
                ChatpHandler_RecvUsername(_Event);
                break;
            case CHATEVENTTYPE_MESSAGE:
                ChatpHandler_Message(_Event);
                break;
            case CHATEVENTTYPE_RECVMSGUPDATE:
                ChatpHandler_RecvMsgUpdate(_Event);
                break;
            case CHATEVENTTYPE_RECVMSGDELETE:
                ChatpHandler_RecvMsgDelete(_Event);
                break;
            case CHATEVENTTYPE_RECVCHANNELS:
                ChatpHandler_RecvChannels(_Event);
                break;
            default:
                fprintf(stderr,
                    "Unknown event type recieved.\n");
                break;
        }
    }
    
    return;
}

void ChatpHandler_Yes(void* _Event) {
    PCHAT_YES Event = _Event;
    
    
    
    return;
}

void ChatpHandler_RecvLogin(void* _Event) {
    return;
}

void ChatpHandler_Message(void* _Event) {
    return;
}

void ChatpHandler_RecvMsgUpdate(void* _Event) {
    return;
}

void ChatpHandler_RecvMsgDelete(void* _Event) {
    return;
}

void ChatpHandler_RecvChannels(void* _Event) {
    PCHAT_CHANNELRECV Event = _Event;
    
    if (!ChatpCtx->Channels) {
        ChatpCtx->Channels = malloc(sizeof(CHATP_CHANNEL));
    } else {
        ChatpCtx->Channels = realloc(ChatpCtx->Channels,
            sizeof(CHATP_CHANNEL) *
            (ChatpCtx->ChannelCount + 1));
    }
    
    PCHATP_CHANNEL NewChannel =
        &ChatpCtx->Channels[ChatpCtx->ChannelCount];
    ChatpCtx->ChannelCount++;
    memset(NewChannel, 0, sizeof(CHATP_CHANNEL));
    
    memcpy(NewChannel->ChannelCryptorKeys,
        Event->ChannelCryptor, sizeof(WORD64) * 4);
    NewChannel->MessageCount = Event->MessageCount;
    wcscpy(NewChannel->ChannelName, Event->ChannelName);
    memcpy(NewChannel->ChannelID, Event->ChannelID,
        sizeof(WORD64) * 2);
    
    PCHAT_REQUESTMSGS MessageRequest = 
        malloc(sizeof(CHAT_REQUESTMSGS));
    memset(MessageRequest, 0, sizeof(CHAT_REQUESTMSGS));
    
    memcpy(MessageRequest->ChannelID, Event->ChannelID,
        sizeof(WORD64) * 2);
    memcpy(MessageRequest->FirstMessageId,
        Event->LastMessageID, sizeof(WORD64) * 2);
    MessageRequest->MessagesToRequest = 50;
    ChatEventSendMessageRequest(MessageRequest);
    free(MessageRequest);
    
    
    free(Event);
    return;
}

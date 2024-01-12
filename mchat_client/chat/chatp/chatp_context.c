//
//  chatp_context.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "chatp.h"
#include "../chat.h"
#include "../../netw/netw.h"

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
    // this won't do anything for now.
    PCHAT_YES Event = _Event;
    // free the event as we own it.
    free(Event);
    return;
}

void ChatpHandler_RecvLogin(void* _Event) {
    // this handler should never recieve this event
    free(_Event);
    return;
}

// qsort's function, sorts the messages such that
// the newest is the first member of the array
int ChatpiTimeCompareFunction(const void* _Arg0,
    const void* _Arg1
) {
    PCHATP_MESSAGE Arg0 = (PCHATP_MESSAGE)_Arg0;
    PCHATP_MESSAGE Arg1 = (PCHATP_MESSAGE)_Arg1;
    
    if (Arg0->SendTime < Arg1->SendTime)
        return 1;
    else if (Arg0->SendTime > Arg1->SendTime)
        return -1;
    
    return 0;
}

void ChatpHandler_Message(void* _Event) {
    PCHAT_MESSAGE Event = _Event;
    
    PCHATP_CHANNEL Channel = NULL;
    for (WORD32 i = 0; i < ChatpCtx->ChannelCount; i++) {
        PCHATP_CHANNEL ThisChannel = 
            &ChatpCtx->Channels[i];
        
        if (ThisChannel->ChannelID[0] == Event->ChannelID[0] &&
            ThisChannel->ChannelID[1] == Event->ChannelID[1]
        ) {
            Channel = ThisChannel;
        }
    }
    
    if (!Channel) {
        free(Event);
        return;
    }

    if (!Channel->Messages) {
        Channel->Messages = malloc(sizeof(CHATP_MESSAGE));
    } else {
        Channel->Messages = realloc(Channel->Messages,
            sizeof(CHATP_MESSAGE) * (Channel->MessageCount + 1));
    }
    
    PCHATP_MESSAGE NewMsg = 
        &Channel->Messages[Channel->MessageCount];
    Channel->MessageCount++;
    memset(NewMsg, 0, sizeof(CHATP_MESSAGE));
    
    wcscpy(NewMsg->AuthorName, Channel->ChannelName);
    NetwDecrypt256(Event->ChatMessageRaw, 2048 * sizeof(wchar_t), 
        Channel->ChannelCryptorKeys);
    wcscpy(NewMsg->Message, Event->ChatMessage);
    memcpy(NewMsg->MessageId, Event->MessageID,
           sizeof(WORD64) * 2);
    NewMsg->SendTime = Event->Sent;
    /*
     Instead of having an 'Edited' variable, a message is assumed to
     be edited if the LastModified and Sent time do not match.
     */
    NewMsg->LastModified = Event->LastEdited;
    
    // Sort the messages such that the newest displays first
    qsort(Channel->Messages, Channel->MessageCount,
        sizeof(CHATP_MESSAGE), ChatpiTimeCompareFunction);
    
    // Update the main messages array
    if (!ChatpCtx->Messages) {
        ChatpCtx->Messages = malloc(sizeof(CHATP_MESSAGE));
    } else {
        ChatpCtx->Messages = realloc(ChatpCtx->Messages,
            sizeof(CHATP_MESSAGE*) * ChatpCtx->MessageCount + 1);
    }
    
    ChatpCtx->Messages[ChatpCtx->MessageCount] = NewMsg;
    ChatpCtx->MessageCount++;
    
    free(Event);
    return;
}

void ChatpHandler_RecvMsgUpdate(void* _Event) {
    PCHAT_MESSAGEUPDATERECV Event = _Event;
    
    for (int i = 0; i < ChatpCtx->MessageCount; i++) {
        PCHATP_MESSAGE Message = ChatpCtx->Messages[i];
        if (Message->MessageId[0] == Event->MessageID[0] &&
            Message->MessageId[1] == Event->MessageID[1]
        ) {
            memcpy(Message->Message, Event->NewBody, 2048 *
                sizeof(wchar_t));
            
            PCHATP_CHANNEL ThisChannel = NULL;
            for (int i = 0; i < ChatpCtx->ChannelCount; i++) {
                PCHATP_CHANNEL Channel = &ChatpCtx->Channels[i];
                if (Channel->ChannelID[0] == Message->ChannelID[0] &&
                    Channel->ChannelID[1] == Message->ChannelID[1]
                ) {
                    ThisChannel = Channel;
                }
            }
            
            if (!ThisChannel) { // invalid message found
                free(Event);
                return;
            }
            
            // decrypt the new message
            NetwDecrypt256(Message->Message,
                2048 * sizeof(wchar_t),
                ThisChannel->ChannelCryptorKeys);
            
            Message->LastModified = Event->EditDate;
        }
    }
    
    free(Event);
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
    
    // Copy the data into the context structure
    memcpy(NewChannel->ChannelCryptorKeys,
        Event->ChannelCryptor, sizeof(WORD64) * 4);
    NewChannel->MessageCount = Event->MessageCount;
    wcscpy(NewChannel->ChannelName, Event->ChannelName);
    memcpy(NewChannel->ChannelID, Event->ChannelID,
        sizeof(WORD64) * 2);
    
    // Request the last 50 messages
    PCHAT_REQUESTMSGS MessageRequest =
        malloc(sizeof(CHAT_REQUESTMSGS));
    memset(MessageRequest, 0, sizeof(CHAT_REQUESTMSGS));
    
    memcpy(MessageRequest->ChannelID, Event->ChannelID,
        sizeof(WORD64) * 2);
    memcpy(MessageRequest->FirstMessageId,
        Event->LastMessageID, sizeof(WORD64) * 2);
    MessageRequest->MessagesToRequest = 50;
    // Send & free this request
    ChatEventSendMessageRequest(MessageRequest);
    free(MessageRequest);
    
    // Free the event as we own it
    free(Event);
    return;
}

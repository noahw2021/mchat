//
//  chatp_context.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "chatp.h"
#include "../chat.h"

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
    return;
}

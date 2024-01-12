//
//  chatp.h
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#ifndef chatp_h
#define chatp_h

#include "../../types.h"
#include <wchar.h>
#include <time.h>

typedef struct _CHATP_MESSAGE {
    wchar_t Message[2048];
    wchar_t AuthorName[128];
    WORD64 MessageId[2];
    time_t SendTime;
}CHATP_MESSAGE, *PCHATP_MESSAGE;

typedef struct _CHATP_CHANNEL {
    wchar_t ChannelName[128];
    PCHATP_MESSAGE Messages;
    WORD32 MessageCount;
}CHATP_CHANNEL, *PCHATP_CHANNEL;

typedef struct _CHATP_REQUEST {
    wchar_t Username[128];
    time_t Sent;
}CHATP_REQUEST, *PCHATP_REQUEST;

typedef struct _CHATP_CTX {
    PCHATP_REQUEST Requests;
    WORD32 RequestCount;
    
    PCHATP_CHANNEL Channels;
    WORD32 ChannelCount;
    
    PCHATP_MESSAGE* Messages;
    WORD32 MessageCount;
}CHATP_CTX, PCHATP_CTX;
extern PCHATP_CTX ChatpCtx;

void ChatpInit(void);
void ChatpShutdown(void);

void ChatpUpdateContext(void);

void ChatpSendMessage(wchar_t* Message);
void ChatpAddUsername(wchar_t* Username);
void ChatpCloseChannel(PCHATP_CHANNEL Channel);
void ChatpDeleteMessage(PCHATP_MESSAGE Message);
void ChatpUpdateMessage(PCHATP_MESSAGE Message, wchar_t* MessageBody);

#endif /* chatp_h */

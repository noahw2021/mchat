//
//  chat.h
//  mchat_client
//
//  Created by Noah Wooten on 12/31/23.
//

#ifndef chat_h
#define chat_h

#include "../types.h"
#include <wchar.h>

/* Client Secret Notation
 0 = Token A
 1 = Token B
 2 = Token C
 3 = Token D
 4 = Key Base (R Constant)
 5 = Nonce Base (R Variable)
 6 = Sender Nonce Seed (R Var)
 7 = Receipt Nonce Seed (R Var)
 */


typedef struct _CHAT_LIST {
    int EventCount;
    int EventTypes[64];
    
    int ExtensionCount;
    int ThisExtension;
}CHAT_LIST, *PCHAT_LIST;

typedef struct _CHAT_LOGIN {
    wchar_t Username[64];
    WORD64 PasswordHash[4];
    WORD64 TemporaryToken[4];
    WORD64 ClientFingerprint[2];
}CHAT_LOGIN, *PCHAT_LOGIN;

typedef struct _CHAT_LOGINRECV {
    BYTE WasSuccess;
    wchar_t ServerMessage[64];
    
    WORD64 MyUserID[4];
    WORD64 MyClientSecret[8];
    
    WORD32 ChannelCount;
}CHAT_LOGINRECV, *PCHAT_LOGINRECV;

typedef struct _CHAT_CHANNELRECV {
    wchar_t ChannelName[128];
    WORD64 MessageCount;
    WORD64 ChannelID[2];
    WORD64 ChannelCryptor[4];
    WORD64 LastMessageID[2];
    
    time_t UserRegister;
    wchar_t UserMOTD[256];
    time_t ChannelCreated;
}CHAT_CHANNELRECV, *PCHAT_CHANNELRECV;

typedef struct _CHAT_REGISTER {
    wchar_t Username[64]; // person's username
    WORD64 PasswordHash[4];
    
    WORD64 UniqueId[4]; // their client generated unique id
    WORD64 KeyBase[16];
    WORD64 ClientSecret[8];
    WORD64 ClientFingerprint[2];
}CHAT_REGISTER, *PCHAT_REGISTER;

typedef struct _CHAT_HELLO {
    wchar_t SendUsername[64];
    wchar_t ReptUsername[64];
    
    WORD64 SenderID[4];
    WORD64 ReceipID[4];
}CHAT_HELLO, *PCHAT_HELLO;

typedef struct _CHAT_YES {
    WORD64 SenderID[4];
    WORD64 ReceipID[4];
}CHAT_YES, *PCHAT_YES;

typedef struct _CHAT_GETUSERNAME {
    wchar_t MyUsername[64];
    WORD64 MyID[4];
}CHAT_GETUSERNAME, *PCHAT_GETUSERNAME;

typedef struct _CHAT_RECVUSERNAME {
    wchar_t RecieptUsername[64];
    WORD64 ReceiptID[4];
}CHAT_RECVUSERNAME, *PCHAT_RECVUSERNAME;

typedef struct _CHAT_MESSAGE {
    union {
        wchar_t ChatMessage[2048];
        WORD64 ChatMessageRaw[256 * sizeof(wchar_t)];
    };
    
    wchar_t Username[128];
    WORD64 AuthorID[4];
    WORD64 MessageID[2];
    WORD64 ChannelID[2];
    time_t Sent;
    time_t LastEdited;
}CHAT_MESSAGE, *PCHAT_MESSAGE;

typedef struct _CHAT_MESSAGEUPDATERECV {
    WORD64 MessageID[2];
    wchar_t NewBody[2048];
    time_t EditDate;
}CHAT_MESSAGEUPDATERECV, *PCHAT_MESSAGEUPDATERECV;

typedef struct _CHAT_MESSAGEDELETERECV {
    WORD64 MessageID[2];
}CHAT_MESSAGEDELETERECV, *PCHAT_MESSAGEDELETERECV;

typedef struct _CHAT_MESSAGEUPDATE  {
    WORD64 MessageID[2];
    wchar_t NewBody[2048];
    BYTE WasDeleted;
}CHAT_MESSAGEUPDATE, *PCHAT_MESSAGEUPDATE;

typedef struct _CHAT_REQUESTMSGS {
    WORD64 ChannelID[2];
    WORD64 FirstMessageId[2];
    WORD64 MessagesToRequest;
}CHAT_REQUESTMSGS, *PCHAT_REQUESTMSGS;

// largest buffer reciept
#define CHATSZ_MAX sizeof(CHAT_MESSAGE) + 1024

void ChatInit(void);
void ChatShutdown(void);

typedef struct _CHAT_CTX {
    WORD64 ClientKey[4];
    PCHAT_LIST PreviousList;
    
    WORD64 MyID[4];
    WORD64 MySecret[8];
    wchar_t MyUsername[64];
    WORD64 ClientFingerprint[2];
}CHAT_CTX, *PCHAT_CTX;
extern PCHAT_CTX ChatCtx;

#define CHATEVENTTYPE_YES           0x01
#define CHATEVENTTYPE_RECVLOGIN     0x02
#define CHATEVENTTYPE_RECVUSERNAME  0x03
#define CHATEVENTTYPE_MESSAGE       0x04
#define CHATEVENTTYPE_RECVMSGUPDATE 0x05
#define CHATEVENTTYPE_RECVMSGDELETE 0x06
#define CHATEVENTTYPE_RECVCHANNELS  0x07

int ChatGetEventList(void);
int ChatGetEventTypes(int i);
void* ChatiGetEvent(int i);

PCHAT_YES          ChatGetEventAsYes(int i);
PCHAT_MESSAGE      ChatGetEventAsMessage(int i);
PCHAT_RECVUSERNAME ChatGetEventAsRecvUsername(int i);
PCHAT_LOGINRECV    ChatGetEventAsRecvLogin(int i);
PCHAT_MESSAGEDELETERECV ChatGetEventAsRecvMessageDelete(int i);
PCHAT_MESSAGEUPDATERECV ChatGetEventAsRecvMessageUpdate(int i);
PCHAT_CHANNELRECV       ChatGetEventAsRecvChannels(int i);

void ChatEventSendRegister(PCHAT_REGISTER Event);
void ChatEventSendHello(PCHAT_HELLO Event);
void ChatEventSendGetUsername(PCHAT_GETUSERNAME Event);
void ChatEventSendMessage(PCHAT_MESSAGE Event);
void ChatEventSendLogin(PCHAT_LOGIN Event);
void ChatEventSendMessageUpdate(PCHAT_MESSAGEUPDATE Event);
void ChatEventSendMessageRequest(PCHAT_REQUESTMSGS Event);

#endif /* chat_h */

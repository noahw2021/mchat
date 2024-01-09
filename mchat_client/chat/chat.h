//
//  chat.h
//  mchat_client
//
//  Created by Noah Wooten on 12/31/23.
//

#ifndef chat_h
#define chat_h

#include "../types.h"

typedef struct _CHAT_REGISTER {
    wchar_t Username[64]; // person's username
    WORD64 UniqueId[4]; // their client generated unique id
    WORD64 KeyBase[16];
}CHAT_REGISTER, *PCHAT_REGISTER;

typedef struct _CHAT_HELLO {
    wchar_t SendUsername[64];
    wchar_t ReptUsername[64];
    
    WORD64 SenderID[4];
    WORD64 ReceipID[4];
    
    BYTE WantsTrustedServerExchange;
    WORD64 MyKeyBase[16];
}CHAT_HELLO, *PCHAT_HELLO;

typedef struct _CHAT_YES {
    WORD64 SenderID[4];
    WORD64 ReceipID[4];
    
    BYTE WantsTrustedServerExchange;
    WORD64 MyKeyBase[16];
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
    wchar_t ChatMessage[16384];
    WORD64 AuthorID[4];
}CHAT_MESSAGE, *PCHAT_MESSAGE;

void ChatInit(void);
void ChatShutdown(void);

int ChatGetEventList(void);
PCHAT_REGISTER    ChatGetEventAsRegister(int i);
PCHAT_HELLO       ChatGetEventAsHello(int i);
PCHAT_YES         ChatGetEventAsYes(int i);
PCHAT_GETUSERNAME ChatGetEventAsUsername(int i);
PCHAT_MESSAGE     ChatGetEventAsMessage(int i);

#endif /* chat_h */

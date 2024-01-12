//
//  ui.h
//  mchat_client
//
//  Created by Noah Wooten on 1/10/24.
//

#ifndef ui_h
#define ui_h

#include "../types.h"
#include "../chat/chatp/chatp.h"
#include <pthread.h>

#define PANE_CHANNEL  1
#define PANE_REQUESTS 2
#define PANE_USERINFO 3

typedef struct _UI_CTX {
    void* Threads;
    pthread_mutex_t AccessMutex;
    
    BYTE WantsShutdown;
    
    wchar_t InputBuffer[2048];
    int InputBufferHeight;
    PCHATP_CHANNEL CurrentChannel;
    int CurrentPane;
}UI_CTX, *PUI_CTX;
extern PUI_CTX UiCtx;

void UiMain(void);
void UiRender(void);

void UiiSetPos(int x, int y);
void UiiClear(void);
void UiiGetSize(int* x, int* y);
void UiiPushColor(int i);
void UiiPopColor(void);

#define UIICOLOR_BOLD 1
#define UIICOLOR_UNDERLINE 4
#define UIICOLOR_BLINK 5
#define UIICOLOR_REDFG 91
#define UIICOLOR_YELLOWFG 93
#define UIICOLOR_GREENFG 92
#define UIICOLOR_REDBG_WHITE 101
#define UIICOLOR_YELLOWBG_WHITE 43
#define UIICOLOR_GREENBG_WHITE 102
#define UIICOLOR_WHITEBG 7

#endif /* ui_h */

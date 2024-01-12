//
//  ui.h
//  mchat_client
//
//  Created by Noah Wooten on 1/10/24.
//

#ifndef ui_h
#define ui_h

#include "../types.h"
#include <pthread.h>

typedef struct _UI_CTX {
    void* Threads;
    pthread_mutex_t AccessMutex;
    
    BYTE WantsShutdown;
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

#endif /* ui_h */

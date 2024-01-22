//
//  panes_channel.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "panes.h"
#include "../ui.h"

#include <stdlib.h>

int UispiTimeCompareFunction(const void* _Arg0,
    const void* _Arg1
) {
    PUI_MESSAGE Arg0 = (PUI_MESSAGE)_Arg0;
    PUI_MESSAGE Arg1 = (PUI_MESSAGE)_Arg1;
    
    if (Arg0->TimeSent < Arg1->TimeSent)
        return 1;
    else if (Arg0->TimeSent > Arg1->TimeSent)
        return -1;
    
    return 0;
}

void UipRenderChannel(PCHATP_CHANNEL Channel) {
    int ConsoleWidth, ConsoleHeight;
    UiiGetSize(&ConsoleWidth, &ConsoleHeight);
    
    pthread_mutex_lock(&UiCtx->AccessMutex);
    if (UiCtx->CurrentPane != PANE_CHANNEL) {
        pthread_mutex_unlock(&UiCtx->AccessMutex);
        return;
    }
    
    pthread_mutex_unlock(&UiCtx->AccessMutex);
    
    for (int i = 0; i < UipCtx->ActiveChannel->MessageCount; i++) {
        PCHATP_MESSAGE Msg = &UipCtx->ActiveChannel->Messages[i];
        
        if (Msg->WasMessageDeleted)
            continue;
        
        UispRenderNewMessage(Msg->AuthorName, Msg->Message,
                             Msg->SendTime);
    }
    
    // sort the messages so '0' is the latest message
    qsort(UipCtx->Messages, UipCtx->MessageCount,
        sizeof(UI_MESSAGE), UispiTimeCompareFunction);
    
    pthread_mutex_lock(&UiCtx->AccessMutex);
    int LinesRemaining = ConsoleHeight - 2;
    int Counter = 1;
    
    // pause stdout buffering
    setvbuf(stdout, NULL, _IONBF, 0);
    
    for (int i = 0; i < UipCtx->MessageCount; i++) {
        PUI_MESSAGE ThisMsg = &UipCtx->Messages[i];
        pthread_mutex_lock(&UiCtx->TextPrintMutex);
        
        wchar_t* Time = malloc(sizeof(wchar_t) * 32);
        struct tm* LocalTime = localtime(&ThisMsg->TimeSent);
        swprintf(Time, 32, L"[%02i:%02i]", LocalTime->tm_hour + 1,
            LocalTime->tm_min + 1);
        
        int ThisCounter = Counter;
        if (!wcscmp(ThisMsg->Author, L"System")) {
            ThisCounter = 0;
        } else {
            Counter++;
        }
        
        BYTE NeedsDrawSeperator = 1;
        if (i != 0 && !wcscmp(UipCtx->Messages[i - 1].Author,
            ThisMsg->Author)
        ) {
            NeedsDrawSeperator = 0;
        }
        
        // draw the author name
        if (NeedsDrawSeperator) {
            wprintf(L"(%i) [%s] (%s)\n", ThisCounter, ThisMsg->Author,
                    Time);
            LinesRemaining--;
        }
            
        // draw the msg body
        LinesRemaining -= (wcslen(ThisMsg->Body) / ConsoleWidth);
        wprintf(L"%s", ThisMsg->Body);
        
        // draw the seperator
        if (NeedsDrawSeperator) {
            for (int i = 0; i < ConsoleWidth; i++)
                fputc('-', stdout);
            LinesRemaining--;
        }
        
        // determine if we can draw this, if not discard stdout
        if (LinesRemaining >= 0) {
            fclose(stdout);
#ifdef _WIN32
            freopen("NUL", "w", stdout);
            fflush(stdout);
            fclose(stdout);
            freopen("CON", "w", stdout);
#else
            freopen("/dev/null", "w", stdout);
            fflush(stdout);
            fclose(stdout);
            freopen("/dev/tty", "w", stdout);
#endif
        } else {
            // we can print the data
            fflush(stdout);
        }
        
        pthread_mutex_unlock(&UiCtx->TextPrintMutex);
        free(Time);
    }
    
    setvbuf(stdout, NULL, _IONBF, 1);
    
    pthread_mutex_unlock(&UiCtx->AccessMutex);
    
    free(UipCtx->Messages);
    UipCtx->MessageCount = 0;
}

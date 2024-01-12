//
//  ui_terminal.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "ui.h"
#include <stdio.h>

void UiiSetPos(int x, int y) {
    printf("\033[%d;%dH", y, x);
    return;
}

void UiiClear(void) {
    fputs("\033[H\033[J", stdout);
    return;
}

void UiiGetSize(int* x, int* y) {
    
}

void UiiSetColor(int Color) {
    printf("\033[%dm\033[m", Color);
}

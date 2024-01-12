//
//  ui_terminal.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "ui.h"
#include <stdio.h>
#include <sys/ioctl.h>

void UiiSetPos(int x, int y) {
    printf("\033[%d;%dH", y, x);
    return;
}

void UiiClear(void) {
    fputs("\033[H\033[J", stdout);
    return;
}

void UiiGetSize(int* x, int* y) {
    struct winsize WindowSize;
    ioctl(0, TIOCGWINSZ, &WindowSize);
    *x = WindowSize.ws_col;
    *y = WindowSize.ws_row;
    return;
}

void UiiPushColor(int i) {
    printf("\033[%dm", i);
}

void UiiPopColor(void) {
    printf("\033[m");
}

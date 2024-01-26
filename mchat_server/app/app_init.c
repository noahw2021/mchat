//
//  app_init.c
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#include "app.h"
#include <stdio.h>
#include <signal.h>

unsigned char WantsQuit;

void InterruptHandler(int _a0) {
    printf("Quitting....\n");
    WantsQuit = 1;
    return;
}

void AppMain(void) {
    signal(SIGINT, InterruptHandler);
    WantsQuit = 0;
    
    printf("Waiting for requests...\n");
    
    while (!WantsQuit)
        usleep(100000);
    
    printf("Server Application Closing...\n");
    return;
}

void AppSetup(void) {
    HttpEp_Register("test", NULL, 0, App_HandleGET);
    return;
}

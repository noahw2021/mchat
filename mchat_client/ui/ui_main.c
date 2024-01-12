//
//  ui_main.c
//  mchat_client
//
//  Created by Noah Wooten on 1/10/24.
//

#include "ui.h"
#include <stdio.h>

void UiMain(void) {
    UiiClear();
    for (int i = 0; i < 100; i++) {
        UiiSetColor(i);
        printf("%i ", i);
    }
    
    return;
}

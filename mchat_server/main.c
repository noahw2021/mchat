//
//  main.c
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#include "app/app.h"
#include "http/http.h"

#include <stdio.h>

int main(int argc, char** argv) {
    printf("MChat Modular Server on Port 4812.\n");
    
    HttpInit(4912);
    
    AppSetup();
    AppMain();
    
    printf("MChat Modular Server Finished.\n");
    return 0;
}

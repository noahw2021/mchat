//
//  netw_init.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "netw.h"
#include <stdlib.h>
#include <string.h>

PNET_CTX NetCtx;
 
void NetwInit(void) {
    NetCtx = malloc(sizeof(NET_CTX));
    memset(NetCtx, 0, sizeof(NET_CTX));
    
    NetCtx->ThisCurlCtx = curl_easy_init();
    NetCtx->ServerAddress = malloc(2048);
    
    // api0/Events_GetObject?
    strcpy(NetCtx->ServerAddress, "http://0.0.0.0/MC_Server10/");
    
    return;
}

void NetwShutdown(void) {
    if (!NetCtx)
        return;
    
    if (NetCtx->ThisCurlCtx)
        curl_easy_cleanup(NetCtx->ThisCurlCtx);
    NetCtx->ThisCurlCtx = NULL;
    
    if (NetCtx->ServerAddress)
        free(NetCtx->ServerAddress);
    NetCtx->ServerAddress = NULL;
    
    free(NetCtx);
    return;
}

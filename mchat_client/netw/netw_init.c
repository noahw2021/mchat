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
    return;
}

void NetwShutdown(void) {
    if (!NetCtx)
        return;
    
    if (NetCtx->ThisCurlCtx)
        curl_easy_cleanup(NetCtx->ThisCurlCtx);
    NetCtx->ThisCurlCtx = NULL;
    
    free(NetCtx);
    return;
}

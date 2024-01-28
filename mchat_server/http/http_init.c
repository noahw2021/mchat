//
//  http_init.c
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#include "http.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include <microhttpd.h>

PHTTP_CTX HttpCtx;

void HttpInit(WORD16 Port) {
    HttpCtx = malloc(sizeof(HTTP_CTX));
    memset(HttpCtx, 0, sizeof(HTTP_CTX));
    
    pthread_mutex_init(&HttpCtx->EndpointsMutex, NULL);
    pthread_mutex_init(&HttpCtx->RequestsMutex, NULL);
    
    HttpCtx->RequestMax = 1024;
    HttpCtx->Requests = 
        malloc(HttpCtx->RequestMax * sizeof(HTTP_REQUEST*));
    
    for (int i = 0; i < HttpCtx->RequestMax; i++) {
        HttpCtx->Requests[i] = malloc(sizeof(HTTP_REQUEST));
    }
    
    HttpCtx->MyDaemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
        Port, NULL, NULL, &HttpiDaemonAnswer, NULL, MHD_OPTION_END);
    
    if (!HttpCtx->MyDaemon) {
        printf("[FATAL]: Failed to load HTTP daemon.\n");
        HttpShutdown();
        return;
    }
    
    return;
}

void HttpShutdown(void) {
    if (!HttpCtx)
        return;
    
    pthread_mutex_destroy(&HttpCtx->EndpointsMutex);
    pthread_mutex_destroy(&HttpCtx->RequestsMutex);
    
    for (int i = 0; i < HttpCtx->EndpointCount; i++)
        free(HttpCtx->Endpoints[i]);
    for (int i = 0; i < HttpCtx->RequestCount; i++)
        free(HttpCtx->Requests[i]);
    
    if (HttpCtx->Endpoints)
        free(HttpCtx->Endpoints);
    if (HttpCtx->Requests)
        free(HttpCtx->Requests);
    
    free(HttpCtx);
    return;
}


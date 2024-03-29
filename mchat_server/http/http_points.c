//
//  http_points.c
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#include "http.h"
#include <stdlib.h>
#include <string.h>

WORD64 HttpEp_Register(const char* Endpoint, PHTTP_ARGUMENT Arguments,
    WORD32 ArgumentCount, HTTPFN_CALLBACK Function
) {
    if (!HttpCtx)
        return 0xFFFFFFFFFFFFFFFF;
    
    pthread_mutex_lock(&HttpCtx->EndpointsMutex);
    if (!HttpCtx->Endpoints) {
        HttpCtx->Endpoints = malloc(sizeof(HTTP_ENDPOINT*));
        HttpCtx->Endpoints[0] = malloc(sizeof(HTTP_ENDPOINT));
    } else {
        HttpCtx->Endpoints = realloc(HttpCtx->Endpoints,
            sizeof(HTTP_ENDPOINT*) * (HttpCtx->EndpointCount + 1));
        HttpCtx->Endpoints[HttpCtx->EndpointCount] =
            malloc(sizeof(HTTP_ENDPOINT));
    }
    pthread_mutex_unlock(&HttpCtx->EndpointsMutex);
    
    PHTTP_ENDPOINT NewPoint = HttpCtx->Endpoints[HttpCtx->EndpointCount];
    HttpCtx->EndpointCount++;
    memset(NewPoint, 0, sizeof(HTTP_ENDPOINT));
    
    // create structure
    pthread_mutex_init(&NewPoint->EndpointModifier, NULL);
    strlcpy(NewPoint->EndpointURL, Endpoint, 128);
    NewPoint->Callback = Function;
    NewPoint->Arguments = Arguments;
    NewPoint->ArgumentCount = ArgumentCount;
    
    return (HttpCtx->EndpointCount - 1);
}

void HttpEp_ModifyArguments(WORD64 Ep, PHTTP_ARGUMENT Arguments,
    WORD32 NewCnt
) {
    if (!HttpCtx)
        return;
    
    if (Ep >= HttpCtx->EndpointCount)
        return;
    
    pthread_mutex_lock(&HttpCtx->EndpointsMutex);
    PHTTP_ENDPOINT ThisEP = HttpCtx->Endpoints[Ep];
    pthread_mutex_unlock(&HttpCtx->EndpointsMutex);
    
    pthread_mutex_lock(&ThisEP->EndpointModifier);
    
    // EP subfunction code
    
    pthread_mutex_unlock(&ThisEP->EndpointModifier);
    
    return;
}

void HttpEp_Disable(WORD64 Ep) {
    if (!HttpCtx)
        return;
    
    if (Ep >= HttpCtx->EndpointCount)
        return;
    
    pthread_mutex_lock(&HttpCtx->EndpointsMutex);
    PHTTP_ENDPOINT ThisEP = HttpCtx->Endpoints[Ep];
    pthread_mutex_unlock(&HttpCtx->EndpointsMutex);
    
    pthread_mutex_lock(&ThisEP->EndpointModifier);
    
    // EP subfunction code
    ThisEP->Enabled = 0;
    
    pthread_mutex_unlock(&ThisEP->EndpointModifier);
    
    return;
}

void HttpEp_Enable(WORD64 Ep) {
    if (!HttpCtx)
        return;
    
    if (Ep >= HttpCtx->EndpointCount)
        return;
    
    pthread_mutex_lock(&HttpCtx->EndpointsMutex);
    PHTTP_ENDPOINT ThisEP = HttpCtx->Endpoints[Ep];
    pthread_mutex_unlock(&HttpCtx->EndpointsMutex);
    
    pthread_mutex_lock(&ThisEP->EndpointModifier);
    
    // EP subfunction code
    ThisEP->Enabled = 1;
    
    pthread_mutex_unlock(&ThisEP->EndpointModifier);
    
    return;
    
}

PHTTP_ENDPOINT HttpEp_GetBase(WORD64 Ep) {
    if (!HttpCtx)
        return NULL;
    
    if (Ep >= HttpCtx->EndpointCount)
        return NULL;
    
    pthread_mutex_lock(&HttpCtx->EndpointsMutex);
    PHTTP_ENDPOINT ThisEP = HttpCtx->Endpoints[Ep];
    pthread_mutex_unlock(&HttpCtx->EndpointsMutex);
    
    return ThisEP;
}

PHTTP_ENDPOINT HttpEp_GetCopy(WORD64 Ep) {
    if (!HttpCtx)
        return NULL;
    
    if (Ep >= HttpCtx->EndpointCount)
        return NULL;
    
    pthread_mutex_lock(&HttpCtx->EndpointsMutex);
    PHTTP_ENDPOINT ThisEP = HttpCtx->Endpoints[Ep];
    pthread_mutex_unlock(&HttpCtx->EndpointsMutex);
    
    PHTTP_ENDPOINT EpBfr = malloc(sizeof(HTTP_ENDPOINT));
    
    pthread_mutex_lock(&ThisEP->EndpointModifier);
    
    // EP subfunction code
    memcpy(EpBfr, ThisEP, sizeof(HTTP_ENDPOINT));
    
    pthread_mutex_unlock(&ThisEP->EndpointModifier);
    
    // the caller owns this structure and must free it
    return EpBfr;
}

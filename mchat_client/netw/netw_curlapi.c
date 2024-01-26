//
//  netw_curlapi.c
//  mchat_client
//
//  Created by Noah Wooten on 1/12/24.
//

#include "netw.h"
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#ifndef __MOTH_NATIVE
#define NETAPI_AGENT "MothChat/1.0 MothClient NonNativeClient"
#else
#define NETAPI_AGENT "MothChat/1.0 MothClient NativeClient"
#endif

unsigned long NetwiGetCBWrite(const void* Buffer, unsigned long Size,
    unsigned long Count, FILE* _File
) {
    return 0;
}



char* NetwGETApi(char* Endpoint) {
    if (!NetCtx)
        return NULL;
    
    char* Url = malloc(strlen(Endpoint) + 2048);
    sprintf(Url, "%s%s", NetCtx->ServerAddress, Endpoint);
    
    curl_easy_setopt(NetCtx->ThisCurlCtx, CURLOPT_URL, Url);
    curl_easy_setopt(NetCtx->ThisCurlCtx, CURLOPT_WRITEFUNCTION,
        NetwiGetCBWrite);
    curl_easy_setopt(NetCtx->ThisCurlCtx, CURLOPT_WRITEDATA, NULL);
    curl_easy_setopt(NetCtx->ThisCurlCtx, CURLOPT_USERAGENT, 
        NETAPI_AGENT);
    
    CURLcode URLCode = curl_easy_perform(NetCtx->ThisCurlCtx);
    
    long HTTPCode;
    curl_easy_getinfo(NetCtx->ThisCurlCtx, CURLINFO_RESPONSE_CODE,
        &HTTPCode);
    
    switch (HTTPCode) {
        case 400:
            NetCtx->LastErrorNumber = NETW_ERROR_INVALIDREQUEST;
            break;
        case 401:
            NetCtx->LastErrorNumber = NETW_ERROR_NOTAUTHORIZED;
            break;
        case 403:
            NetCtx->LastErrorNumber = NETW_ERROR_NOTAUTHORIZED;
            break;
        case 405:
            NetCtx->LastErrorNumber = NETW_ERROR_METHNOTALLOWED;
            break;
        case 408:
            NetCtx->LastErrorNumber = NETW_ERROR_TIMEOUT;
            break;
        case 503:
            NetCtx->LastErrorNumber = NETW_ERROR_OVERLOADED;
            break;
    }
    
    return NULL;
}

void NetwPOSTApi(char* Endpoint, char* Data) {
    return;
}

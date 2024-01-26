//
//  http.h
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#ifndef http_h
#define http_h

#include "../types.h"
#include <pthread.h>

typedef void(* HTTPFN_CALLBACK)(PHTTP_ARGUMENT Arguments,
    WORD32 ArgumentCount, WORD64 RequestId);

typedef struct _HTTP_ARGUMENT {
    char Name[64]; // both
    char* Value; // app
    WORD32 ValueLength; // app
    WORD32 MaxLength; // http
    unsigned char Present; // app
    unsigned char Required; // http
    char CustomError[64]; // http
}HTTP_ARGUMENT, *PHTTP_ARGUMENT;

typedef struct _HTTP_ENDPOINT {
    pthread_mutex_t EndpointModifier;
    char EndpointURL[128];
    
    PHTTP_ARGUMENT Arguments;
    WORD32 ArgumentCount;
    unsigned char Enabled;
}HTTP_ENDPOINT, *PHTTP_ENDPOINT;

typedef struct _HTTP_REQUEST {
    WORD64 RequestId;
    WORD64 RequestIp;
    
    char* RequestURL;
    char* QueryString;
    
    PHTTP_ARGUMENT Arguments;
    WORD32 ArgumentCount;
    
    PHTTP_ENDPOINT ResolvedEndpoint;
    
    char* ResponseBuffer;
    int ReturnCode;
    char* ReturnString;
}HTTP_REQUEST, *PHTTP_REQUEST;

typedef struct _HTTP_CTX {
    pthread_mutex_t EndpointsMutex;
    PHTTP_ENDPOINT Endpoints;
    WORD32 EndpointCount;
    
    pthread_mutex_t RequestsMutex;
    PHTTP_REQUEST Requests;
    WORD64 RequestCount, RequestMax;
    
    void* Context;
}HTTP_CTX, *PHTTP_CTX;

extern PHTTP_CTX HttpCtx;

void HttpInit(void);
void HttpShutdown(void);

WORD64 HttpEp_Register(const char* Endpoint, PHTTP_ARGUMENT Arguments,
    WORD32 ArgumentCount, HTTPFN_CALLBACK Function);
void HttpEp_ModifyArguments(WORD64 Ep, PHTTP_ARGUMENT Arguments,
    WORD32 NewCnt);
void HttpEp_Disable(WORD64 Ep);
void HttpEp_Enable(WORD64 Ep);
PHTTP_ENDPOINT HttpEp_GetBase(WORD64 Ep);
PHTTP_ENDPOINT HttpEp_GetCopy(WORD64 Ep);

void HttpRqe_ServeCode(WORD64 Request, int Code);
void HttpRqe_ServeStatus(WORD64 Request, char* Status);
void HttpRqe_ServeResponse(WORD64 Request, char* Response);
void HttpRqe_Close(WORD64 Request);

PHTTP_ARGUMENT HttpiParseArguments(char* Query, WORD32* ArgumentCount);

#endif /* http_h */

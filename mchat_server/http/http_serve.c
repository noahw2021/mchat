//
//  http_serve.c
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#include "http.h"
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

enum MHD_Result HttpiDaemonAnswer(void* CLS,
    struct MHD_Connection* Connection, const char* URL,
    const char* Method, const char* Version, const char* Upload,
    size_t* UploadSize, void** ConCls
) {
    enum MHD_Result Return;
    
    /*
     Return a 503 if we do not have the slots or capacity to
     handle this request.
     */
    
    printf("Incoming Request: %s %s.\n", Method, URL);
    
    if (HttpCtx->RequestCount >= HttpCtx->RequestMax) {
        const char* Response = "OVERLOADED";
        struct MHD_Response* Resp =
            MHD_create_response_from_buffer(strlen(Response),
            (void*)Response, MHD_RESPMEM_PERSISTENT);
        
        MHD_add_response_header(Resp, "Server",
                "MothServApp/1.0 GenericApp");
        Return = MHD_queue_response(Connection,
            MHD_HTTP_SERVICE_UNAVAILABLE, Resp);
        MHD_destroy_response(Resp);
        
        return Return;
    }
    
    /*
     Allocate a new requests from the pool, and then increment
     the counter.
     */
    PHTTP_REQUEST ThisRequest = HttpCtx->Requests[HttpCtx->RequestCount];
    HttpCtx->RequestCount++;
    
    memset(ThisRequest, 0, sizeof(HTTP_REQUEST));
    // Set the Request ID to a unique number
    ThisRequest->RequestId = HttpCtx->RequestIdCounter;
    ThisRequest->IncomingData = (char*)Upload;
    ThisRequest->IncomingDataSize = *UploadSize;
    HttpCtx->RequestIdCounter++;
    
    // Get the IP address of the client
    union MHD_ConnectionInfo* ConnInfo;
    ConnInfo = (union MHD_ConnectionInfo*)
        MHD_get_connection_info(Connection, 
        MHD_CONNECTION_INFO_CLIENT_ADDRESS);
    struct sockaddr* ClientSock = ConnInfo->client_addr;
    struct sockaddr_in* ClientSkIn = (struct sockaddr_in*)ClientSock;
    unsigned long IpAddr = ClientSkIn->sin_addr.s_addr;
    ThisRequest->RequestIp = IpAddr;
    
    // Create the URL
    ThisRequest->RequestURL = malloc(strlen(URL) + 1);
    strcpy(ThisRequest->RequestURL, URL);
    
    // Create the query string
    if (strstr(URL, "?")) {
        char* QueryAt = strstr(URL, "?");
        ThisRequest->QueryString = malloc(strlen(QueryAt) + 1);
        strcpy(ThisRequest->QueryString, QueryAt + 1);
    } else {
        ThisRequest->QueryString = NULL;
    }
    
    // parse the arguments
    char* QueryString = ThisRequest->QueryString;
    int ArgumentsPresent = 0;
    if (QueryString) {
        while (strstr(QueryString, "&")) {
            if (!ThisRequest->Arguments) {
                ThisRequest->Arguments = malloc(sizeof(HTTP_ARGUMENT));
            } else {
                ThisRequest->Arguments = realloc(ThisRequest->Arguments,
                    sizeof(HTTP_ARGUMENT) *
                    (ThisRequest->ArgumentCount + 1));
            }
            
            PHTTP_ARGUMENT NewArg =
            &ThisRequest->Arguments[ThisRequest->ArgumentCount];
            memset(NewArg, 0, sizeof(HTTP_ARGUMENT));
            ThisRequest->ArgumentCount++;
            
            long NameLength = strstr(QueryString, "=") - QueryString;
            strlcpy(NewArg->Name, QueryString, NameLength);
            /*
             return if we do not have an equals sign or if there is a
             '&' before the '=' (aka a malformed request)
             */
            if (!strstr(QueryString, "=") ||
                (strstr(QueryString, "&") < strstr(QueryString, "="))
                ) {
                const char* Response = "INVALID_REQUEST\r\nBAD_ARG";
                struct MHD_Response* Resp =
                MHD_create_response_from_buffer(strlen(Response),
                                                (void*)Response, MHD_RESPMEM_PERSISTENT);
                
                MHD_add_response_header(Resp, "Server",
                        "MothServApp/1.0 GenericApp");
                Return = MHD_queue_response(Connection,
                                            MHD_HTTP_BAD_REQUEST, Resp);
                MHD_destroy_response(Resp);
                
                free(ThisRequest->Arguments);
                if (ThisRequest->QueryString)
                    free(ThisRequest->QueryString);
                if (ThisRequest->RequestURL)
                    free(ThisRequest->RequestURL);
                
                return Return;
            }
            
            QueryString = strstr(QueryString, "=") + 1;
            NewArg->Value = malloc(64);
            while (*QueryString != '&') {
                // copy the argument
                NewArg->Value[NewArg->ValueLength] = *QueryString;
                NewArg->ValueLength++;
                
                // determine if the argument is over the allocated sz
                if (NewArg->ValueLength > (64 * NewArg->Overflows) - 2) {
                    NewArg->Value = realloc(NewArg->Value,
                                            64 * NewArg->Overflows);
                    
                    NewArg->Overflows++;
                }
                
                // guard against the max size argument
                if (NewArg->ValueLength > 512) {
                    const char* Response = "INVALID_REQUEST\r\n2BG_ARG";
                    struct MHD_Response* Resp =
                    MHD_create_response_from_buffer(strlen(Response),
                        (void*)Response, MHD_RESPMEM_PERSISTENT);
                    
                    MHD_add_response_header(Resp, "Server",
                            "MothServApp/1.0 GenericApp");
                    Return = MHD_queue_response(Connection,
                        MHD_HTTP_BAD_REQUEST, Resp);
                    MHD_destroy_response(Resp);
                    
                    free(ThisRequest->Arguments);
                    if (ThisRequest->QueryString)
                        free(ThisRequest->QueryString);
                    if (ThisRequest->RequestURL)
                        free(ThisRequest->RequestURL);
                    
                    return Return;
                }
            }
            
            ArgumentsPresent++;
        }
    }
    
    // find the endpoint desired

    int SlashCount = 0;
    char* FullURL = ThisRequest->RequestURL;
    while (SlashCount != 1) { // take the URL to after the 1rd slash
        while (*FullURL != '/')
            FullURL++;
        SlashCount++;
    }
    
    PHTTP_ENDPOINT TheEndpoint = NULL;
    for (int i = 0; i < HttpCtx->EndpointCount; i++) {
        PHTTP_ENDPOINT ThisEndpoint = HttpCtx->Endpoints[i];
        HTTP_ENDPOINT EndpointData;
        pthread_mutex_lock(&HttpCtx->EndpointsMutex);
        memcpy(&EndpointData, ThisEndpoint, sizeof(HTTP_ENDPOINT));
        pthread_mutex_unlock(&HttpCtx->EndpointsMutex);
        
        if (strstr(FullURL, EndpointData.EndpointURL)) {
            TheEndpoint = HttpEp_GetCopy(i);
            break;
        }
    }
    
    // endpoint could not be found
    if (!TheEndpoint) {
        const char* Response = "INVALID_REQUEST\r\nBAD_ENDPOINT";
        struct MHD_Response* Resp =
        MHD_create_response_from_buffer(strlen(Response),
            (void*)Response, MHD_RESPMEM_PERSISTENT);
        
        MHD_add_response_header(Resp, "Server",
                "MothServApp/1.0 GenericApp");
        Return = MHD_queue_response(Connection,
            MHD_HTTP_BAD_REQUEST, Resp);
        MHD_destroy_response(Resp);
        
        free(ThisRequest->Arguments);
        if (ThisRequest->QueryString)
            free(ThisRequest->QueryString);
        if (ThisRequest->RequestURL)
            free(ThisRequest->RequestURL);
        
        return Return;
    }
    
    WORD64 ArgumentsPresentField = 0x00;
    WORD64 EveryArgumentField = 0x00;
    for (int i = 0; i < TheEndpoint->ArgumentCount; i++) {
        PHTTP_ARGUMENT ThisArgument = &TheEndpoint->Arguments[i];
        HTTP_ARGUMENT ArgumentCopy;
        
        pthread_mutex_lock(&TheEndpoint->EndpointModifier);
        memcpy(&ArgumentCopy, ThisArgument, sizeof(HTTP_ARGUMENT));
        pthread_mutex_unlock(&TheEndpoint->EndpointModifier);
        
        if (!ArgumentCopy.Required)
            ArgumentsPresentField |= (1LLU << i);
        EveryArgumentField |= (1LLU << i);
        
        for (int a = 0; a < ThisRequest->ArgumentCount; a++) {
            PHTTP_ARGUMENT ItArg = &ThisRequest->Arguments[i];
            
            // if the arg is not required, treat it as if it was
            // there.
            
                
            if (!strcmp(ItArg->Name, ArgumentCopy.Name)) {
                ArgumentsPresentField |= (1LLU << i);
                
                // check if the arg is too big
                if (strlen(ItArg->Value) > ArgumentCopy.MaxLength) {
                    const char* Response =
                        "INVALID_REQUEST\r\nARG_2BIG";
                    struct MHD_Response* Resp =
                    MHD_create_response_from_buffer(strlen(Response),
                        (void*)Response, MHD_RESPMEM_PERSISTENT);
                    
                    MHD_add_response_header(Resp, "Server",
                            "MothServApp/1.0 GenericApp");
                    Return = MHD_queue_response(Connection,
                        MHD_HTTP_BAD_REQUEST, Resp);
                    MHD_destroy_response(Resp);
                    
                    free(ThisRequest->Arguments);
                    if (ThisRequest->QueryString)
                        free(ThisRequest->QueryString);
                    if (ThisRequest->RequestURL)
                        free(ThisRequest->RequestURL);
                    
                    return Return;
                }
            }
        }
    }
    
    // we calculate the arguments we should need, and if
    // its not the same as what we get, something is wrong.
    if (EveryArgumentField != ArgumentsPresentField) {
        const char* Response =
            "INVALID_REQUEST\r\nARG_MISSING";
        struct MHD_Response* Resp =
        MHD_create_response_from_buffer(strlen(Response),
            (void*)Response, MHD_RESPMEM_PERSISTENT);
        
        MHD_add_response_header(Resp, "Server",
                "MothServApp/1.0 GenericApp");
        Return = MHD_queue_response(Connection,
            MHD_HTTP_BAD_REQUEST, Resp);
        MHD_destroy_response(Resp);
        
        free(ThisRequest->Arguments);
        if (ThisRequest->QueryString)
            free(ThisRequest->QueryString);
        if (ThisRequest->RequestURL)
            free(ThisRequest->RequestURL);
        
        return Return;
    }
    
    // call the function
    ThisRequest->ResolvedEndpoint = TheEndpoint;
    HTTPFN_CALLBACK ResolvedCallback = TheEndpoint->Callback;
    ResolvedCallback(ThisRequest->Arguments,
        ThisRequest->ArgumentCount, ThisRequest->RequestId);
    
    // wait for the return
    while (!ThisRequest->RequestCompleted)
        usleep(10000);
    
    // return response to server
    struct MHD_Response* Resp =
    MHD_create_response_from_buffer(strlen(ThisRequest->ResponseBuffer),
        (void*)ThisRequest->ResponseBuffer, MHD_RESPMEM_PERSISTENT);
    
    MHD_add_response_header(Resp, "Server",
            "MothServApp/1.0 GenericApp");
    Return = MHD_queue_response(Connection, ThisRequest->ReturnCode,
        Resp);
    MHD_destroy_response(Resp);
    
    return Return;
}

void HttpRqe_ServeCode(WORD64 Request, int Code) {
    if (!HttpCtx)
        return;
    
    if (Request >= HttpCtx->RequestCount)
        return;
    
    pthread_mutex_lock(&HttpCtx->RequestsMutex);
    PHTTP_REQUEST ThisRequest = HttpCtx->Requests[Request];
    pthread_mutex_unlock(&HttpCtx->RequestsMutex);
    
    // no mutex required because only one thread can access request
    
    ThisRequest->ReturnCode = Code;
    return;
}

void HttpRqe_ServeStatus(WORD64 Request, char* Status) {
    if (!HttpCtx)
        return;
    
    if (Request >= HttpCtx->RequestCount)
        return;
    
    pthread_mutex_lock(&HttpCtx->RequestsMutex);
    PHTTP_REQUEST ThisRequest = HttpCtx->Requests[Request];
    pthread_mutex_unlock(&HttpCtx->RequestsMutex);
    
    // no mutex required because only one thread can access request
    if (ThisRequest->ReturnString)
        free(ThisRequest->ReturnString);
    
    ThisRequest->ReturnString = malloc(strlen(Status) + 1);
    strcpy(ThisRequest->ReturnString, Status);
    
    return;
}

void HttpRqe_ServeResponse(WORD64 Request, char* Response) {
    if (!HttpCtx)
        return;
    
    if (Request >= HttpCtx->RequestCount)
        return;
    
    pthread_mutex_lock(&HttpCtx->RequestsMutex);
    PHTTP_REQUEST ThisRequest = HttpCtx->Requests[Request];
    pthread_mutex_unlock(&HttpCtx->RequestsMutex);
    
    // no mutex required because only one thread can access request
    if (ThisRequest->ReturnString)
        free(ThisRequest->ReturnString);
    
    ThisRequest->ResponseBuffer = malloc(strlen(Response) + 1);
    strcpy(ThisRequest->ResponseBuffer, Response);
    
    return;
}

void HttpRqe_Close(WORD64 Request) {
    if (!HttpCtx)
        return;
    
    if (Request >= HttpCtx->RequestCount)
        return;
    
    pthread_mutex_lock(&HttpCtx->RequestsMutex);
    PHTTP_REQUEST ThisRequest = HttpCtx->Requests[Request];
    pthread_mutex_unlock(&HttpCtx->RequestsMutex);
    
    // no mutex required because only one thread can access request
    ThisRequest->RequestCompleted = 1;
    
    return;
}

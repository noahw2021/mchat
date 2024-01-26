//
//  netw.h
//  mchat_client
//
//  Created by Noah Wooten on 12/31/23.
//

#ifndef netw_h
#define netw_h

#include "../types.h"
#include <curl/curl.h>

typedef struct _NET_CTX {
    int LastErrorNumber;
    CURL* ThisCurlCtx;
    char* ServerAddress;
    
    void* LastBuffer_Get;
    void* LastBuffer_Post;
}NET_CTX, *PNET_CTX;
extern PNET_CTX NetCtx;

void NetwInit(void);
void NetwShutdown(void);

void NetwBase64Encode(char* Buffer, unsigned long BufferSize,
    void* Data, unsigned long DataSize);
void NetwBase64Decode(void* Buffer, unsigned long BufferSize,
    char* Base64String);

void NetwEncrypt256(void* Buffer, unsigned long BufferSize,
    WORD64* Keys);
void NetwDecrypt256(void* Buffer, unsigned long BufferSize,
    WORD64* Keys);

char* NetwGETApi(char* Endpoint);
void NetwPOSTApi(char* Endpoint, char* Data);
int NetwGetLastError(void);

#define NETW_ERROR_SUCCESS        0x00
#define NETW_ERROR_NOTAUTHORIZED  0x01
#define NETW_ERROR_INVALIDREQUEST 0x02
#define NETW_ERROR_NORESPONSE     0x03
#define NETW_ERROR_TIMEOUT        0x04
#define NETW_ERROR_METHNOTALLOWED 0x05
#define NETW_ERROR_OVERLOADED     0x06

#endif /* netw_h */

//
//  netw.h
//  mchat_client
//
//  Created by Noah Wooten on 12/31/23.
//

#ifndef netw_h
#define netw_h

#include "../types.h"

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

#endif /* netw_h */

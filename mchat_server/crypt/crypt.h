//
//  crypt.h
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#ifndef crypt_h
#define crypt_h

#include "../types.h"

void CryptEncryptData(void* Data, WORD32 DataSize, WORD64* Keys256);
void CryptDecryptData(void* Data, WORD32 DataSize, WORD64* Keys256);

char* CryptEncodeBase64(void* Data, WORD32 DataSize);
void CryptDecodeBase64(char* Data, void** DataOut, WORD32* DataSize);

#endif /* crypt_h */

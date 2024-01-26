//
//  crypt_data.c
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#include "crypt.h"
#include <stdlib.h>

// yes - nw 1/26/24
WORD64 LSRGenerator(WORD64 seed, WORD64 _n) {
    WORD64 n = _n;
    
    n ^= ((n << 49) | (n >> 15)) ^ ((n << 24) | (n >> 40));
    n *= seed;
    n ^= n >> 35;
    n *= seed;
    n ^= n >> 28;
    n++;
    return n;
}

void CryptEncryptData(void* Data, WORD32 DataSize, WORD64* Keys256) {
    WORD64 Key64Count = DataSize / sizeof(WORD64);
    WORD64* Keys64 = malloc(sizeof(WORD64) * Key64Count);
    WORD64 FirstPass = 0;
    
    for (int i = 0; i < Key64Count; i++)
        FirstPass ^= Keys64[i];
    
    for (int i = 0; i < Key64Count; i++)
        Keys64[i] = LSRGenerator(FirstPass, FirstPass * i);
    
    WORD16 Key8Count = DataSize % sizeof(WORD64);
    BYTE* Keys8 = malloc(sizeof(BYTE) * Key8Count);
    
    for (int i = 0; i < Key8Count; i++)
        Keys8[i] = LSRGenerator(Keys64[0], ~FirstPass * i);
    
    WORD64* Data64 = (WORD64*)Data;
    BYTE* Data8 = (BYTE*)Data + (sizeof(WORD64) * Key64Count);
    
    for (int i = 0; i < Key64Count; i++)
        Data64[i] ^= Keys64[i];
    for (int i = 0; i < Key8Count; i++)
        Data8[i] ^= Keys8[i];
    
    free(Keys64);
    free(Keys8);
    
    return;
}

void CryptDecryptData(void* Data, WORD32 DataSize, WORD64* Keys256) {
    CryptEncryptData(Data, DataSize, Keys256); // symmetric =D
    return;
}

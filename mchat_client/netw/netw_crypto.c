//
//  netw_crypto.c
//  mchat_client
//
//  Created by Noah Wooten on 1/10/24.
//

#include "netw.h"
#include <stdlib.h>

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

void NetwEncrypt256(void* Buffer, unsigned long BufferSize,
    WORD64* Keys
) {
    WORD64 Key64Count = BufferSize / sizeof(WORD64);
    WORD64* Keys64 = malloc(sizeof(WORD64) * Key64Count);
    WORD64 FirstPass = 0;
    
    for (int i = 0; i < Key64Count; i++)
        FirstPass ^= Keys64[i];
    
    for (int i = 0; i < Key64Count; i++)
        Keys64[i] = LSRGenerator(FirstPass, FirstPass * i);
    
    WORD16 Key8Count = BufferSize % sizeof(WORD64);
    BYTE* Keys8 = malloc(sizeof(BYTE) * Key8Count);
    
    for (int i = 0; i < Key8Count; i++)
        Keys8[i] = LSRGenerator(Keys64[0], ~FirstPass * i);
    
    WORD64* Data64 = (WORD64*)Buffer;
    BYTE* Data8 = (BYTE*)Buffer + (sizeof(WORD64) * Key64Count);
    
    for (int i = 0; i < Key64Count; i++)
        Data64[i] ^= Keys64[i];
    for (int i = 0; i < Key8Count; i++)
        Data8[i] ^= Keys8[i];
    
    free(Keys64);
    free(Keys8);
    
    return;
}

void NetwDecrypt256(void* Buffer, unsigned long BufferSize,
    WORD64* Keys
) {
    // symmetric encryption =D - nw 1/10/24
    NetwEncrypt256(Buffer, BufferSize, Keys);
    
    return;
}

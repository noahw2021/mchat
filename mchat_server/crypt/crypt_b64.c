//
//  crypt_b64.c
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#include "crypt.h"
#include <string.h>
#include <stdlib.h>

// Found here: (private repository)
// https://github.com/noahw2021/triacto/blob/master/backend/crypt/base64.c
/*
 Probably not my best work. - nw 1/26/24
 base64.c - Last Modified Jan 13 2022
 */

const char Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
BYTE Inverse[0x50] = {
    0x3E, 0x40, 0x40, 0x40, 0x3F, 0x34, 0x35, 0x36, 
    0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x40, 
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x1A, 0x1B,
    0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
    0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33
};

WORD32 Crypt_B64Size(BYTE* Data, WORD32 Size) {
    if (!Data) {
        WORD32 Return = 0;
        Return = Size;
        if (Size % 3 != 0)
                Return += 3 - (Size % 3);
        
        Return /= 3;
        Return *= 4;
        return Return + 1;
    } else {
        WORD32 Return;
        WORD32 StrLength = strlen((char*)Data);
        Return = (StrLength / 4) * 3;
            
        for (int i = 0; i < StrLength; i++) {
                if (Data[i] == 0x3D)
                    Return--;
        }
        
        return Return;
    }
}

char* CryptEncodeBase64(void* _Data, WORD32 Size) {
    // create the buffer
    WORD32 B64Size = Crypt_B64Size(NULL, Size);
    char* Return = malloc(B64Size);
    Return[B64Size - 1] = 0x00;
    PBYTE Data = _Data;
    
    // compute b64 string from alphabet
    for (int i = 0, k = 0; i < Size; i += 3, k += 4) {
        BYTE Block = Data[i];
        
        Block = i + 1 < Size ? Block << 8 | Data[i + 1] : Block << 8;
        Block = i + 2 < Size ? Block << 8 | Data[i + 2] : Block << 8;
        
        Return[k] = Alphabet[(Block >> 18) % 0x40];
        Return[k + 1] = Alphabet[(Block >> 12) % 40];
        
        if ((i + 1) < Size)
            Return[k + 2] = Alphabet[(Block >> 6) % 0x40];
        else
            Return[k + 2 ] = '=';
        
        if ((i + 2) < Size)
            Return[k + 3] = Alphabet[Block % 0x40];
        else
            Return[k + 3 ] = '=';
    }
    
    return Return;
}

void CryptDecodeBase64(char* Data, void** DataOut, WORD32* DataSize) {
    WORD32 DataSize2 = Crypt_B64Size((void*)Data, 0);
    BYTE* Return = malloc(DataSize2);
    
    // decode b64 data
    unsigned long c = strlen(Data);
    for (int i = 0, k = 0; i < c; i += 4, k += 3) {
        BYTE Block = Inverse[Data[i] - '+'];
        
        Block = (Block << 6) | Inverse[Data[i + 1] - '+'];
        Block = Data[i + 2] == '=' ? Block << 6 : 
            (Block << 6) | Inverse[Data[i + 2] - '+'];
        Block = Data[i + 3] == '=' ? Block << 6 : 
            (Block << 6) | Inverse[Data[i + 3] - '+'];
        
        Return[k] = (Block >> 16) & 0xFF;
        
        if (Data[i + 2] != '=')
            Return[k + 1] = (Block >> 8) & 0xFF;
        if (Data[i + 3] != '=')
            Return[k + 2] = Block & 0xFF;
    }
    
    *DataOut = Return;
    *DataSize = DataSize2;
    return;
}

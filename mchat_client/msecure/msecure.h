//
//  msecure.h
//  mchat_client
//
//  Created by Noah Wooten on 12/31/23.
//

#ifndef msecure_h
#define msecure_h

#include "../types.h"

typedef struct _MSC_KEY {
    WORD64 KeyBase;
    WORD64 KeyRelocs[32];
    WORD64 KeyHash;
    
    WORD64 KeyData[128];
}MSC_KEY, *PMSC_KEY;

typedef struct _MSC_USER {
    WORD64 UserHash[2];
    STR Alias[64];
}MSC_USER, *PMSC_USER;

typedef struct _MSC_USER_FILE {
    STR Username[64];
    WORD64 PasswordToken[2];
}MSC_USER_FILE, *PMSC_USER_FILE;

// Crypt Functions
void MsccMessageEncrypt(MSC_KEY Key, void* Buffer, WORD32 Length);
void MsccMessageDecrypt(MSC_KEY Key, void* Buffer, WORD32 Length);

// User Functions
void MscuActivateUser(PMSC_USER User, MSC_USER_FILE UserFile);

#endif /* msecure_h */

//
//  app.h
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#ifndef app_h
#define app_h

#include "../types.h"
#include "../http/http.h"

void AppMain(void);
void AppSetup(void);

// test implementation
void App_HandleGET(PHTTP_ARGUMENT Arguments, WORD32 ArgumentCnt,
    WORD64 Req);

#endif /* app_h */

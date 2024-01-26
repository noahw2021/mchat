//
//  app_serve.c
//  mchat_server
//
//  Created by Noah Wooten on 1/26/24.
//

#include "app.h"

void App_HandleGET(PHTTP_ARGUMENT Arguments, WORD32 ArgumentCnt,
    WORD64 Req
) {
    HttpRqe_ServeResponse(Req, "What's up\n");
    HttpRqe_ServeStatus(Req, "OK");
    HttpRqe_ServeCode(Req, 200);
    HttpRqe_Close(Req);

    return;
}

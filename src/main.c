#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "util_common.h"
#include "util_freeipmi.h"
#include "util_plock.h"

#define OEM_NETFN_38 0x38
#define OEM_BMC_RESET_CMD 0x16

int main(int argc, char *const argv[])
{
    ipmi_ctx_t ipmi_ctx = NULL;

    ipmi_ctx = freeipmi_session_create(ipmi_ctx);
    if (!ipmi_ctx)
    {
        log_print(LOG_ERR, "There's an error while Creating free ipmi-session!\n");
        return -1;
    }
    switch_global_iana(0);

    ipmi_cmd_t msg_out;
    memset(&msg_out, 0, sizeof(msg_out));
    msg_out.netfn = OEM_NETFN_38 << 2;
    msg_out.cmd = OEM_BMC_RESET_CMD;
    msg_out.data_len = 0;

    log_print(LOG_NON, "Sending Reset BMC command.\n");

    int resp_cc = send_recv_command(ipmi_ctx, &msg_out);
    if (resp_cc)
    {
        log_print(LOG_ERR, "Failed!, CC 0%x.\n", resp_cc);
    }
    log_print(LOG_NON, "Success!\n");

    if (ipmi_ctx)
    {
        if (freeipmi_session_abort(ipmi_ctx))
            log_print(LOG_ERR, "Can't abort free-ipmi session.\n");
    }

    return 0;
}

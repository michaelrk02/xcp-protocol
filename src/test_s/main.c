#include <signal.h>
#include <stdio.h>

#include <xcp.h>

#define PORT 8080

xcp_server_t *g_server;

xcp_errno_t handle(xcp_server_t *server, xcp_iid_t iid, xcp_mid_t mid, xcp_handle_t *link) {
    if ((iid == 0) && (mid == 0)) {
        xcp_variant_t a, b, c;

        xcp_handle_pop(link, &a);
        xcp_handle_pop(link, &b);

        c.vt = XCP_VT_INT;
        c.val.si = a.val.si + b.val.si;
        printf("%d + %d = %d\n", a.val.si, b.val.si, c.val.si);

        xcp_handle_return(link, XCP_S_OK);
        xcp_handle_push(link, &c);

        return XCP_S_OK;
    }

    return XCP_E_FAIL;
}

void interrupted(int sig) {
    xcp_close(g_server);
    printf("Interrupted\n");
}

int main(int argc, char **argv) {
    signal(SIGINT, interrupted);
    if (XCP_SUCCEEDED(xcp_listen(PORT, handle, &g_server))) {
        printf("Listening...\n");
        xcp_serve(g_server);
    }

    return 0;
}


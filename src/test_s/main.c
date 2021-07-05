#include <signal.h>
#include <stdio.h>

#include <xcp.h>

#define PORT 8080

xcp_server_t *g_server;

xcp_errno_t handle(xcp_server_t *server, xcp_iid_t iid, xcp_mid_t mid, xcp_handle_t *link) {
    if ((iid == 0) && (mid == 0)) {
        xcp_variant_t buf;
        int a, b, c;

        xcp_handle_pop(link, &buf);
        a = xcp_var_get_int(&buf);

        xcp_handle_pop(link, &buf);
        b = xcp_var_get_int(&buf);

        c = a + b;
        printf("%d + %d = %d\n", a, b, c);

        xcp_handle_return(link, XCP_S_OK);

        xcp_var_set_int(&buf, c);
        xcp_handle_push(link, &buf);

        return XCP_S_OK;
    }

    if ((iid == 1) && (mid == 0)) {
        xcp_variant_t buf;
        xcp_sequence_t sqbuf;
        double *va, *vb, dot;

        buf.vt = 0;
        sqbuf.etype = 0;

        xcp_variant_free(&buf);
        xcp_sequence_free(&sqbuf);
        xcp_handle_pop(link, &buf);
        xcp_var_get_sequence(&buf, &sqbuf);
        xcp_seq_get(&sqbuf, XCP_VT_DOUBLE, (void **)&va);

        xcp_variant_free(&buf);
        xcp_sequence_free(&sqbuf);
        xcp_handle_pop(link, &buf);
        xcp_var_get_sequence(&buf, &sqbuf);
        xcp_seq_get(&sqbuf, XCP_VT_DOUBLE, (void **)&vb);

        dot = va[0] * vb[0] + va[1] * vb[1] + va[2] * vb[2];

        printf("Dot product: %.2f\n", dot);
        xcp_handle_return(link, XCP_S_OK);

        xcp_variant_free(&buf);
        xcp_var_set_double(&buf, dot);
        xcp_handle_push(link, &buf);

        xcp_variant_free(&buf);
        xcp_sequence_free(&sqbuf);
        xcp_free(va);
        xcp_free(vb);

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


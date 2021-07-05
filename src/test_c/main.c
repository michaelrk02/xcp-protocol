#include <stdio.h>

#include <xcp.h>

#define PORT 8080

int main(int argc, char **argv) {
    xcp_client_t *client;

    if (XCP_SUCCEEDED(xcp_connect("127.0.0.1", PORT, &client))) {
        xcp_call_t *link;
        xcp_variant_t buf;
        xcp_errno_t err;
        int a, b, c;

        a = 32;
        b = 16;

        if (XCP_SUCCEEDED(xcp_call_begin(client, 0, 0, &link))) {
            xcp_var_set_int(&buf, a);
            xcp_call_push(link, &buf);

            xcp_var_set_int(&buf, b);
            xcp_call_push(link, &buf);

            xcp_call_return(link, &err);

            xcp_call_pop(link, &buf);
            c = xcp_var_get_int(&buf);

            printf("%d + %d = %d\n", a, b, c);
            printf("Err: 0x%08X\n", err);

            xcp_call_end(link);
        }

        if (XCP_SUCCEEDED(xcp_call_begin(client, 1, 0, &link))) {
            double va[3] = {1.0, 2.0, 3.0};
            double vb[3] = {3.0, 2.0, 1.0};
            double dot;
            xcp_sequence_t sqbuf;

            xcp_seq_set(&sqbuf, XCP_VT_DOUBLE, 3, va);
            xcp_var_set_sequence(&buf, &sqbuf);
            xcp_call_push(link, &buf);

            xcp_seq_set(&sqbuf, XCP_VT_DOUBLE, 3, vb);
            xcp_var_set_sequence(&buf, &sqbuf);
            xcp_call_push(link, &buf);

            xcp_call_return(link, &err);

            xcp_variant_free(&buf);
            xcp_call_pop(link, &buf);
            dot = xcp_var_get_double(&buf);

            printf("Dot product: %.2f\n", dot);
            printf("Err: 0x%08X\n", err);

            xcp_call_end(link);
        }

        xcp_disconnect(client);

        return 0;
    }
    printf("Fail!\n");

    return 1;
}


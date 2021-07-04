#include <stdio.h>

#include <xcp.h>

#define PORT 8080

int main(int argc, char **argv) {
    xcp_client_t *client;

    if (XCP_SUCCEEDED(xcp_connect("127.0.0.1", PORT, &client))) {
        xcp_call_t *link;
        xcp_variant_t a, b, c;
        xcp_errno_t err;

        a.vt = XCP_VT_INT;
        a.val.si = 32;

        b.vt = XCP_VT_INT;
        b.val.si = 16;

        xcp_call_begin(client, 0, 0, &link);

        xcp_call_push(link, &a);
        xcp_call_push(link, &b);
        xcp_call_return(link, &err);
        xcp_call_pop(link, &c);

        xcp_call_end(link);

        printf("%d + %d = %d\n", a.val.si, b.val.si, c.val.si);
        printf("Err: 0x%08X\n", err);

        xcp_disconnect(client);

        return 0;
    }
    printf("Fail!\n");

    return 1;
}


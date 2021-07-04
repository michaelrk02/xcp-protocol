#include <stdlib.h>
#include <string.h>

#include "internal.h"

#define xcp_min(a, b) (((a) < (b)) ? (a) : (b))

xcp_errno_t xcp_connect(const char *address, unsigned short port, xcp_client_t **client) {
    xcp_sock_t sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock != XCP_SOCK_INV) {
        xcp_client_t *_client;

        _client = (xcp_client_t *)malloc(sizeof(xcp_client_t));
        strncpy(_client->address, address, xcp_min(strlen(address) + 1, 128));
        _client->port = port;
        _client->addr.sin_family = AF_INET;
        _client->addr.sin_port = htons(port);
        _client->addr.sin_addr.s_addr = inet_addr(address);
        _client->sock = sock;

        *client = _client;

        return XCP_S_OK;
    }

    return XCP_E_FAIL;
}

xcp_errno_t xcp_disconnect(xcp_client_t *client) {
    xcp_sock_close(client->sock);

    return XCP_S_OK;
}


#include <stdlib.h>

#include "internal.h"

xcp_errno_t xcp_call_begin(xcp_client_t *client, xcp_iid_t iid, xcp_mid_t mid, xcp_call_t **link) {
    xcp_call_t *_link;

    _link = (xcp_call_t *)malloc(sizeof(xcp_call_t));
    _link->sock = client->sock;
    if (connect(_link->sock, (struct sockaddr *)&client->addr, sizeof(struct sockaddr_in)) == 0) {
        unsigned int msg = XCP_MSG_CALL_BEGIN;
        *link = _link;

        send(_link->sock, &msg, sizeof(unsigned int), 0);
        send(_link->sock, &iid, sizeof(xcp_iid_t), 0);
        send(_link->sock, &mid, sizeof(xcp_mid_t), 0);
        return XCP_S_OK;
    }

    return XCP_E_FAIL;
}

xcp_errno_t xcp_call_push(xcp_call_t *link, const xcp_variant_t *var) {
    unsigned int msg = XCP_MSG_CALL_PUSH;

    send(link->sock, &msg, sizeof(unsigned int), 0);
    return xcp_marshal(link->sock, var);
}

xcp_errno_t xcp_call_return(xcp_call_t *link, xcp_errno_t *err) {
    unsigned int msg;

    recv(link->sock, &msg, sizeof(unsigned int), 0);
    if (msg == XCP_MSG_HANDLE_RETURN) {
        recv(link->sock, err, sizeof(xcp_errno_t), 0);

        return XCP_S_OK;
    }

    return XCP_E_FAIL;
}

xcp_errno_t xcp_call_pop(xcp_call_t *link, xcp_variant_t *var) {
    unsigned int msg;

    recv(link->sock, &msg, sizeof(unsigned int), 0);
    if (msg == XCP_MSG_HANDLE_PUSH) {
        return xcp_unmarshal(link->sock, var);
    }

    return XCP_E_FAIL;
}

xcp_errno_t xcp_call_end(xcp_call_t *link) {
#ifdef _WIN32
    shutdown(link->sock, SD_BOTH);
#else
    shutdown(link->sock, SHUT_RDWR);
#endif

    free(link);

    return XCP_S_OK;
}


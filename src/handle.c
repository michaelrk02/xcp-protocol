#include "internal.h"

xcp_errno_t xcp_handle_pop(xcp_handle_t *link, xcp_variant_t *var) {
    unsigned int msg;

    recv(link->sock, &msg, sizeof(unsigned int), 0);
    if (msg == XCP_MSG_CALL_PUSH) {
        return xcp_unmarshal(link->sock, var);
    }

    return XCP_E_FAIL;
}

xcp_errno_t xcp_handle_return(xcp_handle_t *link, xcp_errno_t err) {
    unsigned int msg;
    xcp_variant_t verr;

    msg = XCP_MSG_HANDLE_RETURN;
    send(link->sock, &msg, sizeof(unsigned int), 0);
    send(link->sock, &err, sizeof(xcp_errno_t), 0);

    return XCP_S_OK;
}

xcp_errno_t xcp_handle_push(xcp_handle_t *link, const xcp_variant_t *var) {
    unsigned int msg;

    msg = XCP_MSG_HANDLE_PUSH;
    send(link->sock, &msg, sizeof(unsigned int), 0);

    return xcp_marshal(link->sock, var);
}


#include <stdlib.h>
#include <string.h>

#include "internal.h"

xcp_errno_t xcp_marshal(xcp_sock_t sock, const xcp_variant_t *var) {
    send(sock, &var->vt, sizeof(xcp_vartype_t), 0);
    switch (var->vt) {
    case XCP_VT_BOOL:
    case XCP_VT_CHAR:
    case XCP_VT_UCHAR:
        send(sock, &var->val, 1, 0);
        return XCP_S_OK;
    case XCP_VT_SHORT:
    case XCP_VT_USHORT:
        send(sock, &var->val, 2, 0);
        return XCP_S_OK;
    case XCP_VT_ERRNO:
    case XCP_VT_INT:
    case XCP_VT_UINT:
    case XCP_VT_FLOAT:
        send(sock, &var->val, 4, 0);
        return XCP_S_OK;
    case XCP_VT_LONG:
    case XCP_VT_ULONG:
    case XCP_VT_DOUBLE:
        send(sock, &var->val, 8, 0);
        return XCP_S_OK;
    }

    if (var->vt == XCP_VT_STRING) {
        unsigned int length;

        length = strlen(var->val.sz);
        send(sock, &length, sizeof(unsigned int), 0);
        send(sock, var->val.sz, length * sizeof(char), 0);

        return XCP_S_OK;
    }

    if (var->vt == XCP_VT_SEQUENCE) {
        unsigned int i;

        send(sock, &var->val.sq.etype, sizeof(xcp_vartype_t), 0);
        send(sock, &var->val.sq.size, sizeof(unsigned int), 0);
        for (i = 0; i < var->val.sq.size; i++) {
            xcp_marshal(sock, &var->val.sq.data[i]);
        }

        return XCP_S_OK;
    }

    return XCP_E_FAIL;
}

xcp_errno_t xcp_unmarshal(xcp_sock_t sock, xcp_variant_t *var) {
    recv(sock, &var->vt, sizeof(xcp_vartype_t), 0);
    switch (var->vt) {
    case XCP_VT_BOOL:
    case XCP_VT_CHAR:
    case XCP_VT_UCHAR:
        recv(sock, &var->val, 1, 0);
        return XCP_S_OK;
    case XCP_VT_SHORT:
    case XCP_VT_USHORT:
        recv(sock, &var->val, 2, 0);
        return XCP_S_OK;
    case XCP_VT_ERRNO:
    case XCP_VT_INT:
    case XCP_VT_UINT:
    case XCP_VT_FLOAT:
        recv(sock, &var->val, 4, 0);
        return XCP_S_OK;
    case XCP_VT_LONG:
    case XCP_VT_ULONG:
    case XCP_VT_DOUBLE:
        recv(sock, &var->val, 8, 0);
        return XCP_S_OK;
    }

    if (var->vt == XCP_VT_STRING) {
        unsigned int length;

        recv(sock, &length, sizeof(unsigned int), 0);
        var->val.sz = (char *)xcp_alloc((length + 1) * sizeof(char));
        recv(sock, var->val.sz, length * sizeof(char), 0);
        var->val.sz[length] = '\0';
    }

    if (var->vt == XCP_VT_SEQUENCE) {
        recv(sock, &var->val.sq.etype, sizeof(xcp_vartype_t), 0);
        recv(sock, &var->val.sq.size, sizeof(unsigned int), 0);
        if (XCP_SUCCEEDED(xcp_sequence_alloc(&var->val.sq))) {
            unsigned int i;

            for (i = 0; i < var->val.sq.size; i++) {
                xcp_unmarshal(sock, &var->val.sq.data[i]);
            }

            return XCP_S_OK;
        }

        return XCP_E_FAIL;
    }

    return XCP_E_FAIL;
}


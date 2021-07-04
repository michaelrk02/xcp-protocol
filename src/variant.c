#include <stdlib.h>
#include <string.h>

#include "internal.h"

void xcp_variant_free(xcp_variant_t *var) {
    switch (var->vt) {
    case XCP_VT_BOOL:
    case XCP_VT_CHAR:
    case XCP_VT_UCHAR:
        memset(&var->val, 0, 1);
        break;
    case XCP_VT_SHORT:
    case XCP_VT_USHORT:
        memset(&var->val, 0, 2);
        break;
    case XCP_VT_INT:
    case XCP_VT_UINT:
    case XCP_VT_FLOAT:
        memset(&var->val, 0, 4);
        break;
    case XCP_VT_LONG:
    case XCP_VT_ULONG:
    case XCP_VT_DOUBLE:
        memset(&var->val, 0, 8);
        break;
    case XCP_VT_STRING:
        free(var->val.sz);
        var->val.sz = NULL;
        break;
    case XCP_VT_SEQUENCE:
        xcp_sequence_free(&var->val.sq);
        break;
    }
    var->vt = 0;
}

xcp_errno_t xcp_sequence_alloc(xcp_sequence_t *seq) {
    seq->data = (xcp_variant_t *)malloc(seq->size * sizeof(xcp_variant_t));

    return seq->data != NULL ? XCP_S_OK : XCP_E_FAIL;
}

void xcp_sequence_free(xcp_sequence_t *seq) {
    seq->etype = 0;
    seq->size = 0;
    free(seq->data);
    seq->data = NULL;
}


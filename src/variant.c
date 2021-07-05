#include <stdlib.h>
#include <string.h>

#include "internal.h"

void xcp_var_set_errno(xcp_variant_t *var, xcp_errno_t val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_ERRNO;
    var->val.er = val;
}

void xcp_var_set_bool(xcp_variant_t *var, xcp_bool_t val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_BOOL;
    var->val.bo = val;
}

void xcp_var_set_char(xcp_variant_t *var, char val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_CHAR;
    var->val.sc = val;
}

void xcp_var_set_uchar(xcp_variant_t *var, unsigned char val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_UCHAR;
    var->val.uc = val;
}

void xcp_var_set_short(xcp_variant_t *var, short val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_SHORT;
    var->val.ss = val;
}

void xcp_var_set_ushort(xcp_variant_t *var, unsigned short val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_USHORT;
    var->val.us = val;
}

void xcp_var_set_int(xcp_variant_t *var, int val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_INT;
    var->val.si = val;
}

void xcp_var_set_uint(xcp_variant_t *var, unsigned int val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_UINT;
    var->val.ui = val;
}

void xcp_var_set_long(xcp_variant_t *var, char val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_LONG;
    var->val.sl = val;
}

void xcp_var_set_ulong(xcp_variant_t *var, unsigned char val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_ULONG;
    var->val.ul = val;
}

void xcp_var_set_float(xcp_variant_t *var, float val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_FLOAT;
    var->val.fl = val;
}

void xcp_var_set_double(xcp_variant_t *var, double val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_DOUBLE;
    var->val.db = val;
}

void xcp_var_set_string(xcp_variant_t *var, const char *val) {
    unsigned int cap;

    xcp_variant_free(var);
    var->vt = XCP_VT_STRING;

    cap = strlen(val) + 1;
    var->val.sz = (char *)xcp_alloc(cap * sizeof(char));
    strncpy(var->val.sz, val, cap);
}

void xcp_var_set_sequence(xcp_variant_t *var, const xcp_sequence_t *val) {
    xcp_variant_free(var);
    var->vt = XCP_VT_SEQUENCE;
    xcp_sequence_copy(&var->val.sq, val);
}

xcp_errno_t xcp_var_get_errno(const xcp_variant_t *var) {
    return var->val.er;
}

xcp_bool_t xcp_var_get_bool(const xcp_variant_t *var) {
    return var->val.bo;
}

char xcp_var_get_char(const xcp_variant_t *var) {
    return var->val.sc;
}

unsigned char xcp_var_get_uchar(const xcp_variant_t *var) {
    return var->val.uc;
}

short xcp_var_get_short(const xcp_variant_t *var) {
    return var->val.ss;
}

unsigned short xcp_var_get_ushort(const xcp_variant_t *var) {
    return var->val.us;
}

int xcp_var_get_int(const xcp_variant_t *var) {
    return var->val.si;
}

unsigned int xcp_var_get_uint(const xcp_variant_t *var) {
    return var->val.ui;
}

long long xcp_var_get_long(const xcp_variant_t *var) {
    return var->val.sl;
}

unsigned long long xcp_var_get_ulong(const xcp_variant_t *var) {
    return var->val.ul;
}

float xcp_var_get_float(const xcp_variant_t *var) {
    return var->val.fl;
}

double xcp_var_get_double(const xcp_variant_t *var) {
    return var->val.db;
}

void xcp_var_get_string(const xcp_variant_t *var, char **val) {
    char *buf;
    unsigned int cap;

    cap = strlen(var->val.sz) + 1;
    buf = (char *)xcp_alloc(cap * sizeof(char));
    strncpy(buf, var->val.sz, cap);
    *val = buf;
}

void xcp_var_get_sequence(const xcp_variant_t *var, xcp_sequence_t *val) {
    xcp_sequence_copy(val, &var->val.sq);
}

void xcp_seq_set(xcp_sequence_t *seq, xcp_vartype_t vt, unsigned int n, const void *arr) {
    unsigned int i;
    size_t esize = 0;

    switch (vt) {
    case XCP_VT_BOOL:
    case XCP_VT_CHAR:
    case XCP_VT_UCHAR:
        esize = 1;
        break;
    case XCP_VT_SHORT:
    case XCP_VT_USHORT:
        esize = 2;
        break;
    case XCP_VT_ERRNO:
    case XCP_VT_INT:
    case XCP_VT_UINT:
    case XCP_VT_FLOAT:
        esize = 4;
        break;
    case XCP_VT_LONG:
    case XCP_VT_ULONG:
    case XCP_VT_DOUBLE:
        esize = 8;
        break;
    }

    if (esize != 0) {
        xcp_sequence_free(seq);

        seq->etype = vt;
        seq->size = n;
        xcp_sequence_alloc(seq);

        for (i = 0; i < n; i++) {
            seq->data[i].vt = vt;
            memcpy(&seq->data[i].val, (const unsigned char *)arr + i * esize, esize);
        }
    }
}

unsigned int xcp_seq_get(const xcp_sequence_t *seq, xcp_vartype_t vt, void **arr) {
    unsigned int i;
    void *buf;
    size_t esize = 0;

    switch (vt) {
    case XCP_VT_BOOL:
    case XCP_VT_CHAR:
    case XCP_VT_UCHAR:
        esize = 1;
        break;
    case XCP_VT_SHORT:
    case XCP_VT_USHORT:
        esize = 2;
        break;
    case XCP_VT_ERRNO:
    case XCP_VT_INT:
    case XCP_VT_UINT:
    case XCP_VT_FLOAT:
        esize = 4;
        break;
    case XCP_VT_LONG:
    case XCP_VT_ULONG:
    case XCP_VT_DOUBLE:
        esize = 8;
        break;
    }

    if (esize != 0) {
        buf = xcp_alloc(esize * seq->size);
        for (i = 0; i < seq->size; i++) {
            memcpy((unsigned char *)buf + i * esize, &seq->data[i].val, esize);
        }
        *arr = buf;

        return seq->size;
    }

    return 0;
}

void xcp_variant_copy(xcp_variant_t *dst, const xcp_variant_t *src) {
    size_t esize = 0;

    switch (src->vt) {
    case XCP_VT_BOOL:
    case XCP_VT_CHAR:
    case XCP_VT_UCHAR:
        esize = 1;
        break;
    case XCP_VT_SHORT:
    case XCP_VT_USHORT:
        esize = 2;
        break;
    case XCP_VT_ERRNO:
    case XCP_VT_INT:
    case XCP_VT_UINT:
    case XCP_VT_FLOAT:
        esize = 4;
        break;
    case XCP_VT_LONG:
    case XCP_VT_ULONG:
    case XCP_VT_DOUBLE:
        esize = 8;
        break;
    }

    dst->vt = src->vt;
    if (esize != 0) {
        memcpy(&dst->val, &src->val, esize);
    } else {
        if (src->vt == XCP_VT_STRING) {
            char *buf;

            xcp_var_get_string(src, &buf);
            xcp_var_set_string(dst, buf);
            xcp_free(buf);
        } else if (src->vt == XCP_VT_SEQUENCE) {
            xcp_sequence_copy(&dst->val.sq, &src->val.sq);
        }
    }
}

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
    case XCP_VT_ERRNO:
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
        xcp_free(var->val.sz);
        var->val.sz = NULL;
        break;
    case XCP_VT_SEQUENCE:
        xcp_sequence_free(&var->val.sq);
        break;
    }
    var->vt = 0;
}

xcp_errno_t xcp_sequence_alloc(xcp_sequence_t *seq) {
    seq->data = (xcp_variant_t *)xcp_alloc(seq->size * sizeof(xcp_variant_t));

    return seq->data != NULL ? XCP_S_OK : XCP_E_FAIL;
}

void xcp_sequence_free(xcp_sequence_t *seq) {
    if (seq->etype != 0) {
        unsigned int i;

        for (i = 0; i < seq->size; i++) {
            xcp_variant_free(&seq->data[i]);
        }
        xcp_free(seq->data);

        seq->etype = 0;
        seq->size = 0;
        seq->data = NULL;
    }
}

xcp_errno_t xcp_sequence_copy(xcp_sequence_t *dst, const xcp_sequence_t *src) {
    unsigned int i;

    dst->etype = src->etype;
    dst->size = src->size;
    xcp_sequence_alloc(dst);

    for (i = 0; i < dst->size; i++) {
        xcp_variant_copy(&dst->data[i], &src->data[i]);
    }
}


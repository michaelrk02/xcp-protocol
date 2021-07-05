#ifndef __XCP_H__
#define __XCP_H__

#if defined(_MSC_VER)
#define __XCP_IMPORT __declspec(dllimport)
#define __XCP_EXPORT __declspec(dllexport)
#elif defined(_WIN32) && defined(__GNUC__)
#define __XCP_IMPORT __attribute__((dllimport))
#define __XCP_EXPORT __attribute__((dllexport))
#elif defined(__GNUC__)
#define __XCP_IMPORT
#define __XCP_EXPORT __attribute__((visibility("default")))
#else
#error Unsupported compiler. Use GNU C or Microsoft C compiler
#endif

#ifdef XCPBUILD
#define XCPAPI __XCP_EXPORT
#else
#define XCPAPI __XCP_IMPORT
#endif

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
** Utility macros
*/

#define XCP_MAKE_DWORD(hi, lo)  ((unsigned int)((((hi) & 0xFFFF) << 16) | ((lo) & 0xFFFF)))
#define XCP_MAKE_WORD(hi, lo)   ((unsigned short)((((hi) & 0xFF) << 8) | ((lo) & 0xFF)))

#define XCP_MAKE_FOURCC(a, b, c, d) \
    XCP_MAKE_DWORD( \
        XCP_MAKE_WORD(d, c), \
        XCP_MAKE_WORD(b, a) \
    )

#define xcp_alloc(size)     malloc(size)
#define xcp_free(ptr)       free(ptr)

/*
** Error type
*/

typedef unsigned int xcp_errno_t;
#define XCP_MAKE_ERRNO(facility, code) XCP_MAKE_DWORD((facility) & 0xFFF, code)

#define XCP_ERRLOC(err)     (((err) >> 28) & 0xF)
#define XCP_ERRFAC(err)     (((err) >> 16) & 0xFFF)
#define XCP_ERRCODE(err)    ((err) & 0xFFFF)

#define XCP_ERR_REMOTE      0xF
#define XCP_ERR_LOCAL       0x0

#define XCP_SUCCEEDED(err)  (XCP_ERRCODE(err) == 0x0000)
#define XCP_FAILED(err)     (XCP_ERRCODE(err) != 0x0000)

#define XCP_S_OK    XCP_MAKE_ERRNO(0x000, 0x0000)
#define XCP_E_FAIL  XCP_MAKE_ERRNO(0x000, 0xFFFF)

/*
** Variant type
*/

typedef struct __xcp_string_t xcp_string_t;
typedef struct __xcp_variant_t xcp_variant_t;
typedef struct __xcp_sequence_t xcp_sequence_t;

typedef unsigned int xcp_vartype_t;
#define XCP_VTYPE(x, y) XCP_MAKE_FOURCC('X', 'T', x, y)
#define XCP_VT_ANY      XCP_VTYPE('a', 'n')
#define XCP_VT_ERRNO    XCP_VTYPE('e', 'r')
#define XCP_VT_BOOL     XCP_VTYPE('b', 'o')
#define XCP_VT_CHAR     XCP_VTYPE('s', 'c')
#define XCP_VT_UCHAR    XCP_VTYPE('u', 'c')
#define XCP_VT_SHORT    XCP_VTYPE('s', 's')
#define XCP_VT_USHORT   XCP_VTYPE('u', 's')
#define XCP_VT_INT      XCP_VTYPE('s', 'i')
#define XCP_VT_UINT     XCP_VTYPE('u', 'i')
#define XCP_VT_LONG     XCP_VTYPE('s', 'l')
#define XCP_VT_ULONG    XCP_VTYPE('u', 'l')
#define XCP_VT_FLOAT    XCP_VTYPE('f', 'l')
#define XCP_VT_DOUBLE   XCP_VTYPE('d', 'b')
#define XCP_VT_STRING   XCP_VTYPE('s', 'z')
#define XCP_VT_SEQUENCE XCP_VTYPE('s', 'q')

typedef unsigned char xcp_bool_t;
#define XCP_TRUE    1
#define XCP_FALSE   0

#pragma pack(push, 1)
struct __xcp_sequence_t {
    xcp_vartype_t etype;
    unsigned int size;
    xcp_variant_t *data;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct __xcp_variant_t {
    xcp_vartype_t vt;
    union __xcp_varvalue_t {
        xcp_errno_t er;
        xcp_bool_t bo;
        char sc;
        unsigned char uc;
        short ss;
        unsigned short us;
        int si;
        unsigned int ui;
        long long sl;
        unsigned long long ul;
        float fl;
        double db;
        char *sz;
        xcp_sequence_t sq;
    } val;
};
#pragma pack(pop)

/* Variant setters */
extern XCPAPI void xcp_var_set_errno(xcp_variant_t *var, xcp_errno_t val);
extern XCPAPI void xcp_var_set_bool(xcp_variant_t *var, xcp_bool_t val);
extern XCPAPI void xcp_var_set_char(xcp_variant_t *var, char val);
extern XCPAPI void xcp_var_set_uchar(xcp_variant_t *var, unsigned char val);
extern XCPAPI void xcp_var_set_short(xcp_variant_t *var, short val);
extern XCPAPI void xcp_var_set_ushort(xcp_variant_t *var, unsigned short val);
extern XCPAPI void xcp_var_set_int(xcp_variant_t *var, int val);
extern XCPAPI void xcp_var_set_uint(xcp_variant_t *var, unsigned int val);
extern XCPAPI void xcp_var_set_long(xcp_variant_t *var, char val);
extern XCPAPI void xcp_var_set_ulong(xcp_variant_t *var, unsigned char val);
extern XCPAPI void xcp_var_set_float(xcp_variant_t *var, float val);
extern XCPAPI void xcp_var_set_double(xcp_variant_t *var, double val);
extern XCPAPI void xcp_var_set_string(xcp_variant_t *var, const char *val);
extern XCPAPI void xcp_var_set_sequence(xcp_variant_t *var, const xcp_sequence_t *val);

/* Variant getters */
extern XCPAPI xcp_errno_t xcp_var_get_errno(const xcp_variant_t *var);
extern XCPAPI xcp_bool_t xcp_var_get_bool(const xcp_variant_t *var);
extern XCPAPI char xcp_var_get_char(const xcp_variant_t *var);
extern XCPAPI unsigned char xcp_var_get_uchar(const xcp_variant_t *var);
extern XCPAPI short xcp_var_get_short(const xcp_variant_t *var);
extern XCPAPI unsigned short xcp_var_get_ushort(const xcp_variant_t *var);
extern XCPAPI int xcp_var_get_int(const xcp_variant_t *var);
extern XCPAPI unsigned int xcp_var_get_uint(const xcp_variant_t *var);
extern XCPAPI long long xcp_var_get_long(const xcp_variant_t *var);
extern XCPAPI unsigned long long xcp_var_get_ulong(const xcp_variant_t *var);
extern XCPAPI float xcp_var_get_float(const xcp_variant_t *var);
extern XCPAPI double xcp_var_get_double(const xcp_variant_t *var);
extern XCPAPI void xcp_var_get_string(const xcp_variant_t *var, char **val);
extern XCPAPI void xcp_var_get_sequence(const xcp_variant_t *var, xcp_sequence_t *val);

/* Sequence operations */
extern XCPAPI void xcp_seq_set(xcp_sequence_t *seq, xcp_vartype_t vt, unsigned int n, const void *arr);
extern XCPAPI unsigned int xcp_seq_get(const xcp_sequence_t *seq, xcp_vartype_t vt, void **arr);

/* Memory management */

extern XCPAPI void xcp_variant_copy(xcp_variant_t *dst, const xcp_variant_t *src);
extern XCPAPI void xcp_variant_free(xcp_variant_t *var);

extern XCPAPI xcp_errno_t xcp_sequence_alloc(xcp_sequence_t *seq);
extern XCPAPI void xcp_sequence_free(xcp_sequence_t *seq);
extern XCPAPI xcp_errno_t xcp_sequence_copy(xcp_sequence_t *dst, const xcp_sequence_t *src);

/*
** Remote procedure calls
*/

typedef struct __xcp_client_t xcp_client_t;
typedef struct __xcp_server_t xcp_server_t;
typedef struct __xcp_call_t xcp_call_t;
typedef struct __xcp_handle_t xcp_handle_t;

typedef unsigned int xcp_iid_t;
typedef unsigned int xcp_mid_t;

typedef xcp_errno_t (*xcp_handler_t)(xcp_server_t *server, xcp_iid_t iid, xcp_mid_t mid, xcp_handle_t *link);

extern XCPAPI xcp_errno_t xcp_connect(const char *address, unsigned short port, xcp_client_t **client);
extern XCPAPI xcp_errno_t xcp_disconnect(xcp_client_t *client);

extern XCPAPI xcp_errno_t xcp_call_begin(xcp_client_t *client, xcp_iid_t iid, xcp_mid_t mid, xcp_call_t **link);
extern XCPAPI xcp_errno_t xcp_call_push(xcp_call_t *link, const xcp_variant_t *var);
extern XCPAPI xcp_errno_t xcp_call_return(xcp_call_t *link, xcp_errno_t *err);
extern XCPAPI xcp_errno_t xcp_call_pop(xcp_call_t *link, xcp_variant_t *var);
extern XCPAPI xcp_errno_t xcp_call_end(xcp_call_t *link);

extern XCPAPI xcp_errno_t xcp_listen(unsigned short port, xcp_handler_t handler, xcp_server_t **server);
extern XCPAPI xcp_errno_t xcp_serve(xcp_server_t *server);
extern XCPAPI xcp_errno_t xcp_close(xcp_server_t *server);

extern XCPAPI xcp_errno_t xcp_handle_pop(xcp_handle_t *link, xcp_variant_t *var);
extern XCPAPI xcp_errno_t xcp_handle_return(xcp_handle_t *link, xcp_errno_t err);
extern XCPAPI xcp_errno_t xcp_handle_push(xcp_handle_t *link, const xcp_variant_t *var);


#ifdef __cplusplus
}
#endif

#endif

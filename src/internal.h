#ifndef __XCP_INTERNAL_H__
#define __XCP_INTERNAL_H__

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <xcp.h>

/*
** Socket library
*/

#ifdef _WIN32
typedef SOCKET xcp_sock_t;
#define XCP_SOCK_INV    INVALID_SOCKET
#else
typedef int xcp_sock_t;
#define XCP_SOCK_INV    (-1)
#endif

extern XCPAPI int xcp_sock_init(void);
extern XCPAPI int xcp_sock_quit(void);

extern XCPAPI int xcp_sock_close(xcp_sock_t sock);

/*
** Mutual exclusion
*/

#define xcp_mtx_lock(m)
#define xcp_mtx_unlock(m)

/*
** Data types
*/

struct __xcp_client_t {
    char address[128];
    unsigned short port;
    struct sockaddr_in addr;
    xcp_sock_t sock;
};

struct __xcp_server_t {
    unsigned short port;
    xcp_handler_t handler;
    struct sockaddr_in addr;
    xcp_sock_t sock;
    xcp_bool_t running;
};

struct __xcp_call_t {
    xcp_sock_t sock;
};

struct __xcp_handle_t {
    xcp_sock_t sock;
};

/*
** RPC stream message codes
*/

#define XCP_MSG(x, y)           XCP_MAKE_FOURCC('X', 'M', x, y)
#define XCP_MSG_CALL_BEGIN      XCP_MSG('c', 'b')
#define XCP_MSG_CALL_PUSH       XCP_MSG('c', 'u')
#define XCP_MSG_HANDLE_RETURN   XCP_MSG('h', 'r')
#define XCP_MSG_HANDLE_PUSH     XCP_MSG('h', 'u')

/*
** Variant marshalling
*/

extern XCPAPI xcp_errno_t xcp_marshal(xcp_sock_t sock, const xcp_variant_t *var);
extern XCPAPI xcp_errno_t xcp_unmarshal(xcp_sock_t sock, xcp_variant_t *var);

#endif

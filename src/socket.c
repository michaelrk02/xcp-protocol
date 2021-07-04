#include "internal.h"

int xcp_sock_init(void) {
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
    return 0;
#endif
}

int xcp_sock_quit(void) {
#ifdef _WIN32
    return WSACleanup();
#else
    return 0;
#endif
}

int xcp_sock_close(xcp_sock_t sock) {
    int status;

#ifdef _WIN32
    status = shutdown(sock, SD_BOTH);
    if (status == 0) {
        status = closesocket(sock);
    }
#else
    status = shutdown(sock, SHUT_RDWR);
    if (status == 0) {
        status = close(sock);
    }
#endif

    return status;
}


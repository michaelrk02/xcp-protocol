#include <stdlib.h>

#include "internal.h"

xcp_errno_t xcp_listen(unsigned short port, xcp_handler_t handler, xcp_server_t **server) {
    xcp_sock_t sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock != XCP_SOCK_INV) {
        int opt;

        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int)) == 0) {
            xcp_server_t *_server;

            _server = (xcp_server_t *)malloc(sizeof(xcp_server_t));
            _server->port = port;
            _server->handler = handler;
            _server->addr.sin_family = AF_INET;
            _server->addr.sin_addr.s_addr = INADDR_ANY;
            _server->addr.sin_port = htons(port);

            if (bind(sock, (struct sockaddr *)&_server->addr, sizeof(struct sockaddr_in)) == 0) {
                if (listen(sock, 0) == 0) {
                    _server->sock = sock;
                    _server->running = XCP_TRUE;
                    *server = _server;

                    return XCP_S_OK;
                }
            }

            free(_server);
        }
        xcp_sock_close(sock);
    }

    return XCP_E_FAIL;
}

xcp_errno_t xcp_serve(xcp_server_t *server) {
    while (XCP_TRUE) {
        xcp_bool_t running;

        xcp_mtx_lock(server->mtx);
        running = server->running;
        xcp_mtx_unlock(server->mtx);

        if (running) {
            xcp_handle_t link;

            link.sock = accept(server->sock, NULL, NULL);
            if (link.sock != XCP_SOCK_INV) {
                unsigned int msg = 0;

                recv(link.sock, &msg, sizeof(unsigned int), 0);
                if (msg == XCP_MSG_CALL_BEGIN) {
                    xcp_iid_t iid;
                    xcp_mid_t mid;

                    recv(link.sock, &iid, sizeof(xcp_iid_t), 0);
                    recv(link.sock, &mid, sizeof(xcp_mid_t), 0);
                    server->handler(server, iid, mid, &link);
                }

                xcp_sock_close(link.sock);
            }
        } else {
            break;
        }
    }

    return XCP_S_OK;
}

xcp_errno_t xcp_close(xcp_server_t *server) {
    xcp_mtx_lock(&server->mtx);
    server->running = XCP_FALSE;
    xcp_mtx_unlock(&server->mtx);

    xcp_sock_close(server->sock);

    free(server);

    return XCP_S_OK;
}


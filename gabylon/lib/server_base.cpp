#include <sys/socket.h>
#include <cstring>
#include <vector>
#include <zconf.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server_base.h"

int ServerBase::start(uint16_t port) {
    struct sockaddr_in addr = {0};

    int listeningSocket;
    listeningSocket = socket(AF_INET, SOCK_STREAM, 0);

    int ret;
    int on = 1;
    ret = setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (ret < 0) {
        printf("SO_REUSEADDR error! %d\n", ret);
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // addr.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr.s_addr));
    printf("allow: %s\n", inet_ntoa(addr.sin_addr));
    ret = bind(listeningSocket, (struct sockaddr *)&addr, sizeof(addr));

    if (ret < 0) {
        printf("Bind error!: %d\n", ret);
        return 1;
    }

    ret = listen(listeningSocket, 5);

    if (ret < 0) {
        printf("Listen error!: %d\n", ret);
        return 1;
    }

    auto count = 0;

    while(true) {
        printf("connection accepted\n");
        acceptRequest(listeningSocket);
        printf("connection closed\n");

        count++;

        if (count >= 10) break;
    }
    close(listeningSocket);

    return 0;
}

ssize_t ServerBase::error(int socket, const std::string &humanErrorMessage) {
    std::string message = "ERROR: " + humanErrorMessage + "\n\n";
    return reply(socket, message);
}

ssize_t ServerBase::reply(int socket, const std::string &message) {
    auto ret = send(socket, message.c_str(), message.length(), 0);
    if (ret < 0) {
        return ret;
    } else {
        return 0;
    }
}

ssize_t ServerBase::ok(int socket) {
    return reply(socket, "OK");
}

ssize_t ServerBase::ng(int socket) {
    return reply(socket, "NG");
}

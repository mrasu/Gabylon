#include <utility>
#include <utility>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <zconf.h>
#include <fstream>
#include "gabylon_server.h"
#include "file_data.h"

GabylonServer::GabylonServer(std::string base_path): base_path(std::move(base_path)) {}

int GabylonServer::start() {
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
    addr.sin_port = htons(12345);
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

    // while(true) {
    struct sockaddr_in client = {0};
    socklen_t len = sizeof(client);

    int socket;
    socket = accept(listeningSocket, (struct sockaddr *) &client, &len);

    std::cout << ("accepted connection") << std::endl;

    FileData *data = this->readSocket(socket);
    if (data == nullptr) {
        printf("Cannot load data from client\n");
        return 1;
    }

    data->write(this->base_path);

    close(socket);
    // }
    close(listeningSocket);

    return 0;
}

FileData *GabylonServer::readSocket(int socket) {
    char buffer[50];
    ssize_t sret;

    bool line_break_found = false;
    std::string file_path;
    std::vector<char> file_content;
    while(true) {
        memset(buffer, 0, sizeof(buffer));
        sret = read(socket, buffer, sizeof(buffer));
        if (sret < 0) {
            printf("read error!: %zd", sret);
            return nullptr;
        } else if (sret == 0) {
            break;
        }
        printf("reading...\n");

        int i = 0;
        if (!line_break_found) {
            for (; i < sret; i++) {
                char ch = buffer[i];
                if (ch == '\n') {
                    i++;
                    line_break_found = true;
                    break;
                }
                file_path += ch;
            }
        }

        for (; i < sret; i++) {
            file_content.push_back(buffer[i]);
        }
    }
    printf("Finish! size: %lu, path: %s\n", file_content.size(), file_path.c_str());

    FileMeta *meta = new FileMeta(file_path);
    FileContent *content = new FileContent(file_content);

    return new FileData(meta, content);
}

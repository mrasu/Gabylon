#include <utility>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <zconf.h>
#include <cstring>
#include <vector>
#include "gabylon_meta_server.h"
#include "file_meta.h"
#include "meta_message.h"
#include "../exceptions/invalid_method_exception.h"
#include "../exceptions/invalid_body_exception.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

GabylonMetaServer::GabylonMetaServer():
    metaMap(std::unordered_map<std::string, FileMeta*>()),
    writingMetas(std::unordered_map<std::string, FileMeta*>()) {}

int GabylonMetaServer::start() {
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

    auto count = 0;

    while(true) {
        struct sockaddr_in client = {0};
        socklen_t len = sizeof(client);

        int socket;
        socket = accept(listeningSocket, (struct sockaddr *) &client, &len);

        std::cout << ("accepted connection") << std::endl;

        try {
            ret = handleSocketData(socket);
            if (ret != 0) {
                printf("error happen: %d\n", ret);
            }
        } catch (std::exception& e) {
            printf("Error: %s\n", e.what());
        }

        close(socket);
        count++;

        if (count >= 2) break;
    }
    close(listeningSocket);

    return 0;
}

int GabylonMetaServer::handleSocketData(int socket) {
    auto message = readSocketData(socket);
    if (!message) {
        return 1;
    }

    if (message->method == MetaMessageMethod::CREATE) {
        auto meta = FileMeta::dumpMessage(message->body);
        auto uuid = boost::uuids::random_generator()();
        // auto uuidString = boost::lexical_cast<std::string>(fileId);
        std::string uuidString;
        std::stringstream ss;
        ss << uuid;
        ss >> uuidString;

        writingMetas[uuidString] = meta;
        auto ret = send(socket, uuidString.c_str(), uuidString.length(), 0);

        if (ret > 0) {
            printf("fileId sent: %s(%zd)\n", uuidString.c_str(), ret);
            return 0;
        } else {
            printf("*******");
            return -1;
        }
    } else {
        return 1;
    }
}

MetaMessage* GabylonMetaServer::readSocketData(int socket) {
    char buffer[50];
    memset(buffer, 0, sizeof(buffer));

    ssize_t sret;

    std::vector<char> messages;
    bool is_previous_new_line = false;
    bool reaches_end = false;
    while(true) {
        sret = read(socket, buffer, sizeof(buffer));

        if (sret < 0) {
            printf("read error!: %zd", sret);
            return nullptr;
        } else if (sret == 0) {
            break;
        }

        for (int i = 0; i < sret; i++) {
            if (buffer[i] == '\n') {
                if (is_previous_new_line) {
                    reaches_end = true;
                    break;
                }
                is_previous_new_line = true;
            } else if (is_previous_new_line){
                is_previous_new_line = false;
            }
            messages.push_back(buffer[i]);
        }
        if (reaches_end) {
            break;
        }
    }

    return MetaMessage::dumpText(&messages[0]);
}

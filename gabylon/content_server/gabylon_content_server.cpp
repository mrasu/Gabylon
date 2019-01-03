#include "gabylon_content_server.h"
#include "content_message_method.h"
#include "write_info.h"
#include "../exceptions/invalid_method_exception.h"
#include <boost/filesystem.hpp>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>


GabylonContentServer::GabylonContentServer(const std::string path, sockaddr_in metaServerAddr): basePath(path), metaServerAddr(metaServerAddr) {
}

int GabylonContentServer::start() {
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
    addr.sin_port = htons(12346);
    inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr.s_addr));
    ret = bind(listeningSocket, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        printf("Bind error! %d\n", ret);
        return 1;
    }

    ret = listen(listeningSocket, 5);
    if (ret < 0) {
        printf("Listen ereor! %d\n", ret);
        return 1;
    }

    int count = 0;
    while(true) {
        struct sockaddr_in client = {0};
        socklen_t len = sizeof(client);

        int socket = accept(listeningSocket, (sockaddr*)&client, &len);

        printf("accept connection!\n");

        try {
            ret = handleSocketData(socket);
            if (ret != 0) {
                printf("error happen: %d\n", ret);
            }
            printf("finish: handleSocketData\n");
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

int GabylonContentServer::handleSocketData(int socket) {
    auto message = readSocketContentMessage(socket);
    if (!message) {
        return 1;
    }

    int ret = 0;
    if (message->method == ContentMessageMethod::WRITE) {
        auto writeInfo = WriteInfo::dumpMessage(message->body);
        if (!writeInfo) {
            return 1;
        }

        bool storableData = validateFileidForWriting(writeInfo);
        if (!storableData) {
            printf("Validation to save failed!");
            ret = reply(socket, false, "Validation to save failed");
            if (ret < 0) {
                printf("Failed to send validation failure: %d\n", ret);
            }
            return 1;
        }

        ret = reply(socket, true, "Validation passed");
        if (ret < 0) {
            printf("Failed to send validation success: %d\n", ret);
            return 1;
        }

        ret = saveStream(socket, writeInfo);
        if (ret < 0) {
            printf("Failed to save file data: %d\n", ret);
            return 1;
        }

        ret = reply(socket, true, "File saved");
        if (ret < 0) {
            printf("Failed to send file saved (but saved in disk): %d\n", ret);
            return 0;
        }
    } else {
        throw InvalidMethodException(("Invalid method: " + std::to_string(message->method)).c_str());
    }

    return 0;
}

ContentMessage *GabylonContentServer::readSocketContentMessage(int socket) {
    char buffer[50];
    memset(buffer, 0, sizeof(buffer));

    ssize_t sret;
    std::vector<char>messages;
    bool is_previous_new_line = false;
    bool reaches_end = false;

    while(true) {
        sret = read(socket, buffer, sizeof(buffer));
        if (sret < 0) {
            printf("read error!: %zd\n", sret);
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
            } else if (is_previous_new_line) {
                is_previous_new_line = false;
            }
            messages.push_back(buffer[i]);
        }

        if (reaches_end) {
            break;
        }
    }

    return ContentMessage::dumpText(&messages[0]);
}

bool GabylonContentServer::validateFileidForWriting(WriteInfo *) {
    // TODO
    return true;
}

int GabylonContentServer::reply(int socket, bool succeeds, const std::string &humanMessage) {
    std::string message;
    if (succeeds) {
        message = "SUCCEEDS: " + humanMessage;
    } else {
        message = "FAIL: " + humanMessage; 
    }
    auto ret = send(socket, message.c_str(), message.length(), 0);
    if (ret < 0) {
        return (int)ret;
    } else {
        return 0;
    }
}

int GabylonContentServer::saveStream(int socket, WriteInfo* info) {
    char buffer[50];
    memset(buffer, 0, sizeof(buffer));

    std::vector<char> file_content;
    long readSize = 0;
    while(true) {
        auto ret = read(socket, buffer, sizeof(buffer));
        if (ret < 0) {
            printf("read error! %zd", ret);
            return 1;
        } else if (ret == 0) {
            break;
        }
        printf("reading...\n");

        for(auto i = 0; i < ret; i++) {
            file_content.push_back(buffer[i]);
        }

        readSize += ret;
        if (info->size <= readSize) {
            break;
        }
    }

    std::ofstream outputStream;
    outputStream.open(basePath + info->path, std::ifstream::binary | std::ifstream::out);
    outputStream.write(&file_content[0], file_content.size());
    outputStream.close();

    return 0;
}

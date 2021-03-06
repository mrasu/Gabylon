#include "gabylon_content_server.h"
#include "content_message_method.h"
#include "write_info.h"
#include "../server_exceptions/invalid_method_exception.h"
#include "../internal_client/gabylon_meta_server_client.h"
#include "../internal_client/client_exception.h"
#include <boost/filesystem.hpp>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>


GabylonContentServer::GabylonContentServer(const std::string path, sockaddr_in metaServerAddr): basePath(path), metaServerAddr(metaServerAddr) {
}

void GabylonContentServer::acceptRequest(int listeningSocket) {
    struct sockaddr_in client = {0};
    socklen_t len = sizeof(client);

    int acceptedSocket = accept(listeningSocket, (sockaddr*)&client, &len);

    try {
        auto ret = handleSocketData(acceptedSocket);
        if (ret != 0) {
            printf("error happen: %d\n", ret);
        }
        printf("finish: handleSocketData\n");
    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    close(acceptedSocket);
}

int GabylonContentServer::handleSocketData(int socket) {
    auto message = readSocketContentMessage(socket);
    if (!message) {
        return 1;
    }

    int ret = 0;
    ssize_t sret;
    if (message->method == ContentMessageMethod::WRITE) {
        auto writeInfo = WriteInfo::dumpMessage(message->body);
        if (!writeInfo) {
            return 1;
        }

        try {
            auto isWritable = validateFileidForWriting(writeInfo);
            if (!isWritable) {
                printf("Validation to save failed!\n");
                sret = error(socket, "Validation to save failed");
                if (sret < 0) {
                    printf("Failed to send validation failure: %d\n", ret);
                }
                return 1;
            }
        } catch (ClientException& e) {
            printf("Failed to check FileId: %s\n", e.what());
            sret = error(socket, "Validation to save failed");
            if (sret < 0) {
                printf("Failed to send validation failure: %d\n", ret);
            }
            return 1;
        }

        sret = ok(socket);
        if (sret < 0) {
            printf("Failed to send validation success: %d\n", ret);
            return 1;
        }

        ret = saveStream(socket, writeInfo);
        if (ret < 0) {
            printf("Failed to save file data: %d\n", ret);
            return 1;
        }

        sret = ok(socket);
        if (sret < 0) {
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
    std::string message;
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
            message += buffer[i];
        }

        if (reaches_end) {
            break;
        }
    }

    return ContentMessage::dumpText(message);
}

bool GabylonContentServer::validateFileidForWriting(WriteInfo *writeInfo) {
    auto client = GabylonMetaServerClient(metaServerAddr);
    return client.isFileCreating(writeInfo->fileId);
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
    std::string filePath = basePath + info->path + "=" + std::to_string(info->from);
    outputStream.open(filePath, std::ifstream::binary | std::ifstream::out);
    outputStream.write(&file_content[0], file_content.size());
    outputStream.close();

    return 0;
}

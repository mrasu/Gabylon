#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <zconf.h>
#include <arpa/inet.h>
#include <vector>
#include "gabylon/lib/message_body.h"
#include "gabylon/lib/socket_read_writer.h"

struct File {
    std::string path;
    long size;
};

struct ServerInfo {
    long firstBytePosition;
    std::string host;
    uint16_t port;
};

struct FileSaveInfo {
    std::string fileId;
    std::vector<ServerInfo*> servers;
};

std::string sendMeta(File*);
FileSaveInfo* parseMetaResponse(std::string response);
int sendData(FileSaveInfo*, File*);

int main() {
    std::cout << "Start client!" << std::endl;

    // std::string in_file_path = "test_short.txt";
    // std::string in_file_path = "test_long.txt";
    std::string in_file_path = "Crown.png";

    std::ifstream is("./input/" + in_file_path, std::ifstream::binary);
    if (!is) {
        printf("Failed to dumpMessage stream");
        return 1;
    }
    is.seekg(0, std::fstream::end);
    const auto end = is.tellg();
    is.clear();
    is.seekg(0, std::fstream::beg);
    const auto start = is.tellg();
    const auto size = end - start;
    is.close();

    auto file = new File{in_file_path,size};

    auto response = sendMeta(file);
    auto info = parseMetaResponse(response);

    return sendData(info, file);
}

std::string sendMeta(File *file) {
    printf("sending meta...\n");
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    // setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    // setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    struct sockaddr_in server = {};
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    ssize_t ret;
    std::string message = "CREATE\nPath:/" + file->path + "\nSize:" + std::to_string(file->size) + "\n\n";
    ret = send(sock, message.c_str(), message.length(), 0);
    if (ret < 0) {
        printf("CREATE: Failed to send file path!: %zd", ret);
        return "";
    }
    printf("Sent path!\n");

    auto reader = new SocketReadWriter(sock);
    auto response = reader->readReceivedText();
    printf("FIN! (id: %s)\n", response.c_str());

    close(sock);

    return response;
}

FileSaveInfo* parseMetaResponse(std::string response) {
    auto message = MessageBody::parseMessage(response);
    auto fileId = message->get("FileId");
    if (fileId.empty()) {
        return nullptr;
    }
    auto info = new FileSaveInfo{};
    info->fileId = fileId;

    auto server = message->get("Servers");
    if (server.empty()) {
        return nullptr;
    }

    auto sectionIndex = 0;
    std::string byteString;
    std::string host;
    std::string portString;
    for (auto ch: server) {
        if (sectionIndex == 0) {
            if (ch == '=') {
                sectionIndex++;
            }
        } else if (sectionIndex == 1) {
            if (ch == '=') {
                sectionIndex++;
            } else {
                byteString += ch;
            }
        } else if (sectionIndex == 2) {
            if (ch == ':') {
                sectionIndex++;
            } else {
                host += ch;
            }
        } else if (sectionIndex == 3) {
            if (ch == ';') {
                auto byte = std::stol(byteString);
                auto port = (uint16_t)std::stoi(portString);

                auto s = new ServerInfo{byte, host, port};
                info->servers.push_back(s);
                byteString = "";
                host = "";
                portString = "";

                sectionIndex = 0;
            } else {
                portString += ch;
            }
        }
    }

    return info;
}

int sendData(FileSaveInfo *saveInfo, File *file) {
    auto path = file->path;
    std::ifstream is("./input/" + path, std::ifstream::binary);
    if (!is) {
        printf("Failed to dumpMessage stream");
        return 1;
    }

    long fileReadPosition = 0;
    for (auto i = 0; i < saveInfo->servers.size(); i++) {
        int sock;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        ServerInfo* serverInfo = saveInfo->servers[i];
        struct sockaddr_in server = {};
        server.sin_family = AF_INET;
        server.sin_port = htons(serverInfo->port);
        server.sin_addr.s_addr = inet_addr(serverInfo->host.c_str());

        printf("Connecting to %s:%d...\n", serverInfo->host.c_str(), serverInfo->port);
        connect(sock, (struct sockaddr *)&server, sizeof(server));

        auto nextServerBytePosition = (i+1 < saveInfo->servers.size()) ? saveInfo->servers[i+1]->firstBytePosition : file->size;
        auto size = nextServerBytePosition - fileReadPosition;
        auto startPosition = serverInfo->firstBytePosition;

        ssize_t ret;
        std::string message = "WRITE\nFileId:" + saveInfo->fileId +
                "\nSize:" + std::to_string(size) +
                "\nPath:" + path +
                "\nFrom:" + std::to_string(startPosition) + "\n\n";

        ret = send(sock, message.c_str(), message.length(), 0);
        if (ret < 0) {
            printf("WRITE: Failed to send file path!: %zd", ret);
            return 1;
        }

        char buffer[1000];
        ret = read(sock, buffer, sizeof(buffer));
        if (ret < 0) {
            printf("Failed to read: %zd", ret);
            return 1;
        }
        printf("%s\n", buffer);

        char fileBuffer[1000];
        memset(fileBuffer, 0, sizeof(fileBuffer));
        std::string text;
        while(!is.eof() && fileReadPosition < nextServerBytePosition) {
            auto readSize = fileReadPosition + sizeof(fileBuffer) < nextServerBytePosition ? sizeof(fileBuffer): nextServerBytePosition - fileReadPosition;
            is.read(fileBuffer, readSize);
            auto actualReadCount = (size_t)is.gcount();
            ret = send(sock, fileBuffer, actualReadCount, 0);
            if (ret < 0) {
                printf("Failed to send!: %zd", ret);
                is.close();
                return -1;
            }

            fileReadPosition += actualReadCount;
            printf("sending...\n");
        }

        memset(buffer, 0, sizeof(buffer));
        ret = read(sock, buffer, sizeof(buffer));
        if (ret < 0) {
            printf("Failed to read: %zd", ret);
            return 1;
        }
        printf("%s\n", buffer);
        close(sock);
    }

    is.close();

    printf("FIN!\n");
}

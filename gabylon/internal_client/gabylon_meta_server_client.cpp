#include <zconf.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include "gabylon_meta_server_client.h"
#include "client_exception.h"
#include "../lib/socket_read_writer.h"

GabylonMetaServerClient::GabylonMetaServerClient(sockaddr_in metaServerAddr) {
    auto creatingSocket = socket(AF_INET, SOCK_STREAM, 0);
    timeval timeout{};
    timeout.tv_sec = 1;

    setsockopt(creatingSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    connect(creatingSocket, (sockaddr *)&metaServerAddr, sizeof(metaServerAddr));

    clientSocket = creatingSocket;
}

GabylonMetaServerClient::~GabylonMetaServerClient() {
    if (clientSocket != 0) {
        close(clientSocket);
    }
};

bool GabylonMetaServerClient::isFileCreating(std::string fileId) {
    std::string message = "CHECK_CREATING\nFileId:" + fileId + "\n\n";
    auto ret = send(clientSocket, message.c_str(), message.length(), 0);
    if (ret < 0) {
        throw ClientException(("CHECK_CREATING failed to send server: " + std::to_string(ret)).c_str());
    }

    auto reader = SocketReadWriter(clientSocket);
    auto response = reader.readReceivedText();
    if (response == "OK") {
        return true;
    } else if (response == "NG") {
        return false;
    } else {
        printf("isFileCreating failed!: %s\n", response.c_str());
        return false;
    }
}

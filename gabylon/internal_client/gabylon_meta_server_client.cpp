#include <zconf.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include "gabylon_meta_server_client.h"
#include "client_exception.h"

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

    auto response = readResponse();
    if (response == "OK") {
        return true;
    } else if (response == "NG") {
        return false;
    } else {
        printf("isFileCreating failed!: %s\n", response.c_str());
        return false;
    }
}

std::string GabylonMetaServerClient::readResponse() {
    char buffer[50];
    memset(buffer, 0, sizeof(buffer));

    std::vector<char> response;
    auto is_previous_new_line = false;
    while(true) {
        auto ret = read(clientSocket, buffer, sizeof(buffer));
        if (ret < 0) {
            throw ClientException(("Failed to read response: " + std::to_string(ret)).c_str());
        } else if (ret == 0) {
            break;
        }

        for (int i = 0; i < ret; i++) {
            response.push_back(buffer[i]);
            if (buffer[i] == '\n') {
                if (is_previous_new_line) {
                    break;
                } else {
                    is_previous_new_line = true;
                }
            } else if (is_previous_new_line) {
                is_previous_new_line = false;
            }
        }
    }

    while(!response.empty() && response.back() == '\n') {
        response.pop_back();
    }

    if (response.empty()) {
        return "";
    } else {
        return std::string(response.begin(), response.end());
    }
}

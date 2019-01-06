#include <utility>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <zconf.h>
#include <cstring>
#include <vector>
#include "gabylon_meta_server.h"
#include "file_meta_message.h"
#include "meta_message.h"
#include "../server_exceptions/invalid_method_exception.h"
#include "../server_exceptions/invalid_body_exception.h"
#include "check_creating_message.h"
#include "../lib/socket_read_writer.h"
#include "file_meta.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

GabylonMetaServer::GabylonMetaServer():
    metaMap(std::unordered_map<std::string, FileMeta*>()),
    writingMetas(std::unordered_map<std::string, FileMeta*>()) {

    ContentServerInfo *contentServers[] = {
            new ContentServerInfo{"127.0.0.1", 12346},
            new ContentServerInfo{"127.0.0.1", 12346}
    };

    for(auto i = 0; i < 100; i++) {
        auto mod = i % 2;
        contentServerAssignMap[i] = contentServers[mod];
    }
}

void GabylonMetaServer::acceptRequest(int listeningSocket) {
    sockaddr_in client = {0};
    socklen_t len = sizeof(client);

    auto acceptedSocket = accept(listeningSocket, (struct sockaddr *) &client, &len);

    printf("connection accepted\n");

    try {
        auto ret = handleSocketData(acceptedSocket);
        if (ret != 0) {
            printf("error happen: %d\n", ret);
        }
    } catch (InvalidMethodException& e) {
        error(acceptedSocket, e.what());
        printf("Error: %s\n", e.what());
    } catch (InvalidBodyException& e) {
        error(acceptedSocket, e.what());
        printf("Error: %s\n", e.what());
    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    close(acceptedSocket);
}

int GabylonMetaServer::handleSocketData(int socket) {
    auto message = readSocketData(socket);
    if (!message) {
        return 1;
    }

    if (message->method == MetaMessageMethod::CREATE) {
        auto metaMessage = FileMetaMessage::dumpMessage(message->body);
        auto uuid = boost::uuids::random_generator()();
        // auto uuidString = boost::lexical_cast<std::string>(fileId);
        std::stringstream ss;
        ss << uuid;
        auto uuidString = ss.str();

        writingMetas[uuidString] = metaMessage->getFileMeta();
        std::string serverMessage = "";

        auto currentServerIndex = 0;
        for (auto firstByte = 0; firstByte < metaMessage->size; firstByte += 1000) {
            auto server = contentServerAssignMap[currentServerIndex];
            serverMessage += "byte=" + std::to_string(firstByte) + "=" + server->str() + ";";

            currentServerIndex++;
        }
        auto fileMessage = "FileId:" + uuidString + "\nServers:" + serverMessage + "\n\n";
        auto ret = send(socket, fileMessage.c_str(), fileMessage.length(), 0);

        if (ret > 0) {
            printf("fileId sent: %s(%zd)\n", uuidString.c_str(), ret);
            return 0;
        } else {
            printf("*******");
            return -1;
        }
    } else if (message->method == MetaMessageMethod::CHECK_CREATING) {
        auto creatingMessage = CheckCreatingMessage::dumpMessage(message->body);
        auto fileId = creatingMessage->fileId;

        auto search = writingMetas.find(fileId);
        if (search == writingMetas.end()) {
            printf("fileId not found: %s\n", fileId.c_str());
            auto ret = ng(socket);
            if (ret < 0) {
                printf("Failed to send fileId not found response");
                return 1;
            }
        } else {
            printf("fileId found: %s\n", fileId.c_str());
            auto ret = ok(socket);
            if (ret < 0) {
                printf("Failed to send fileId found response");
                return 1;
            }
        }

        return 0;
    } else {
        auto ret = ServerBase::error(socket, "Undefined method");
        if (ret < 0) {
            printf("Failed to reply failure: Undefined method");
        }
        return 1;
    }
}

MetaMessage* GabylonMetaServer::readSocketData(int socket) {
    auto reader = SocketReadWriter(socket);
    auto message = reader.readReceivedText();
    return MetaMessage::dumpText(message);
}

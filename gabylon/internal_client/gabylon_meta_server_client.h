#ifndef CLIENT_GABYLON_META_SERVER_CLIENT_H
#define CLIENT_GABYLON_META_SERVER_CLIENT_H


#include <string>

class GabylonMetaServerClient {
private:
    int clientSocket;
    std::string readResponse();
public:
    GabylonMetaServerClient(sockaddr_in metaServerAddr);
    ~GabylonMetaServerClient();
    bool isFileCreating(std::string fileId);
};


#endif //CLIENT_GABYLON_META_SERVER_CLIENT_H

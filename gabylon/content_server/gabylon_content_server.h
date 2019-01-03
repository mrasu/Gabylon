#ifndef CLIENT_GABYLON_CONTENT_SERVER_H
#define CLIENT_GABYLON_CONTENT_SERVER_H


#include <string>
#include <netinet/in.h>
#include "content_message.h"
#include "write_info.h"

class GabylonContentServer {
private:
    std::string basePath;
    sockaddr_in metaServerAddr;
    int handleSocketData(int);
    ContentMessage* readSocketContentMessage(int);
    int reply(int, bool, const std::string&);
    bool validateFileidForWriting(WriteInfo*);
    int saveStream(int, WriteInfo*);
public:
    explicit GabylonContentServer(std::string, sockaddr_in);
    int start();
};


#endif //CLIENT_GABYLON_CONTENT_SERVER_H

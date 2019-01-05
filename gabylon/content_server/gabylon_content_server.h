#ifndef CLIENT_GABYLON_CONTENT_SERVER_H
#define CLIENT_GABYLON_CONTENT_SERVER_H


#include <string>
#include <netinet/in.h>
#include "content_message.h"
#include "write_info.h"
#include "../lib/server_base.h"

class GabylonContentServer: public ServerBase {
private:
    std::string basePath;
    sockaddr_in metaServerAddr;
    int handleSocketData(int);
    ContentMessage* readSocketContentMessage(int);
    bool validateFileidForWriting(WriteInfo*);
    int saveStream(int, WriteInfo*);
protected:
    void acceptRequest(int listeningSocket) override;
public:
    explicit GabylonContentServer(std::string, sockaddr_in);
};


#endif //CLIENT_GABYLON_CONTENT_SERVER_H

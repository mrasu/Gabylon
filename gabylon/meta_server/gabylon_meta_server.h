#ifndef CLIENT_GABYLON_META_SERVER_H
#define CLIENT_GABYLON_META_SERVER_H


#include <string>
#include <unordered_map>
#include "file_meta.h"
#include "meta_message.h"
#include "../lib/server_base.h"

class GabylonMetaServer: public ServerBase {
private:
    std::unordered_map<std::string, FileMeta*> metaMap;
    std::unordered_map<std::string, FileMeta*> writingMetas;
    int handleSocketData(int socket);
    MetaMessage* readSocketData(int socket);
protected:
    void acceptRequest(int listeningSocket) override;
public:
    GabylonMetaServer();
};


#endif //CLIENT_GABYLON_META_SERVER_H

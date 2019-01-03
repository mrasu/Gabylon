#ifndef CLIENT_GABYLON_META_SERVER_H
#define CLIENT_GABYLON_META_SERVER_H


#include <string>
#include <unordered_map>
#include "file_meta.h"
#include "meta_message.h"

class GabylonMetaServer {
private:
    std::unordered_map<std::string, FileMeta*> metaMap;
    std::unordered_map<std::string, FileMeta*> writingMetas;
    int handleSocketData(int socket);
    MetaMessage* readSocketData(int socket);
public:
    GabylonMetaServer();
    int start();
};


#endif //CLIENT_GABYLON_META_SERVER_H

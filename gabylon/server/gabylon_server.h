#ifndef CLIENT_GABYLON_SERVER_H
#define CLIENT_GABYLON_SERVER_H


#include <string>
#include "file_data.h"

class GabylonServer {
private:
    const std::string base_path;
    FileData *readSocket(int socket);
public:
    explicit GabylonServer(std::string base_path);
    int start();
};


#endif //CLIENT_GABYLON_SERVER_H

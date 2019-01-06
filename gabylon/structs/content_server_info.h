#ifndef CLIENT_CONTENT_SERVER_INFO_H
#define CLIENT_CONTENT_SERVER_INFO_H


#include <stdint-gcc.h>

struct ContentServerInfo {
    std::string host;
    uint16_t port;
    std::string str();
};


#endif //CLIENT_CONTENT_SERVER_INFO_H

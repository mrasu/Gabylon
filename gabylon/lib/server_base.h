#ifndef CLIENT_SERVER_BASE_H
#define CLIENT_SERVER_BASE_H


#include <string>

class ServerBase {
private:
    ssize_t reply(int socket, const std::string &message);
protected:
    ssize_t error(int socket, const std::string &humanErrorMessage);
    ssize_t ok(int socket);
    ssize_t ng(int socket);
    virtual void acceptRequest(int listeningSocket) = 0;
public:
    int start(uint16_t port);
};


#endif //CLIENT_SERVER_BASE_H

#ifndef CLIENT_SERVER_BASE_H
#define CLIENT_SERVER_BASE_H


#include <string>

class ServerBase {
private:
    ssize_t reply(int socket, const std::string &message);
protected:
    std::string readSentData(int socket);
    ssize_t error(int socket, const std::string &humanErrorMessage);
    ssize_t ok(int socket);
    ssize_t ng(int socket);
public:
    virtual int start() = 0;
};


#endif //CLIENT_SERVER_BASE_H

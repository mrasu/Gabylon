#ifndef CLIENT_SOCKET_READ_WRITER_H
#define CLIENT_SOCKET_READ_WRITER_H


#include <string>

class SocketReadWriter {
public:
    int socketFileDescriptor;

    explicit SocketReadWriter(int socket);
    std::string readReceivedText();
};


#endif //CLIENT_SOCKET_READ_WRITER_H

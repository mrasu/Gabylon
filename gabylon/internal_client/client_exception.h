#ifndef CLIENT_CLIENT_EXCEPTION_H
#define CLIENT_CLIENT_EXCEPTION_H


#include <stdexcept>

class ClientException: public std::runtime_error {
public:
    explicit ClientException(const char* message);
};


#endif //CLIENT_CLIENT_EXCEPTION_H

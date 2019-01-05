#ifndef CLIENT_INVALID_METHOD_EXCEPTION_H
#define CLIENT_INVALID_METHOD_EXCEPTION_H

#include <stdexcept>

class InvalidMethodException: public std::runtime_error {
public:
    explicit InvalidMethodException(const char* message);
};

#endif //CLIENT_INVALID_METHOD_EXCEPTION_H

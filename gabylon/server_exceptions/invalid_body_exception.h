#ifndef CLIENT_INVALID_BODY_EXCEPTION_H
#define CLIENT_INVALID_BODY_EXCEPTION_H

#include <stdexcept>

class InvalidBodyException: public std::runtime_error {
public:
    explicit InvalidBodyException(const char* message);
};


#endif //CLIENT_INVALID_BODY_EXCEPTION_H

#include "invalid_body_exception.h"

InvalidBodyException::InvalidBodyException(const char *message): std::runtime_error(message){}

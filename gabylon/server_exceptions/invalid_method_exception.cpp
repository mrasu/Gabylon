#include <stdexcept>
#include "invalid_method_exception.h"

InvalidMethodException::InvalidMethodException(const char *message):
    std::runtime_error(message) {}

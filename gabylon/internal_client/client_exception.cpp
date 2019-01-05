#include "client_exception.h"

ClientException::ClientException(const char *message): std::runtime_error(message) {}

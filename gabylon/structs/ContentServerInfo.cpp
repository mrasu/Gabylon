

#include <string>
#include "content_server_info.h"

std::string ContentServerInfo::str() {
    return host + ":" + std::to_string(port);
}

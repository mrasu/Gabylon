#include <utility>
#include <unordered_map>

#include "write_info.h"
#include "../server_exceptions/invalid_body_exception.h"
#include "../lib/message_body.h"

WriteInfo *WriteInfo::dumpMessage(const std::string textMessage) {
    auto message = MessageBody::parseMessage(textMessage);

    auto fileId = message->get("FileId");
    if (fileId.empty()) {
        return nullptr;
    }

    auto size = message->getLong("Size");
    if (size == -1) {
        return nullptr;
    }

    auto path = message->get("Path");
    if (path.empty()) {
        return nullptr;
    }

    auto from = message->getLong("From");
    if (from == -1) {
        return nullptr;
    }

    return new WriteInfo(fileId, size, path, from);
}


WriteInfo::WriteInfo(const std::string fileId, long size, const std::string path, long from):
    fileId(fileId),
    size(size),
    path(path),
    from(from) {

}


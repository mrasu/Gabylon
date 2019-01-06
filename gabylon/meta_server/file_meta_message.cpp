#include <utility>

#include "file_meta_message.h"
#include "../server_exceptions/invalid_body_exception.h"

FileMetaMessage::FileMetaMessage(std::string path, long size): path(std::move(path)), size(size) {}

FileMetaMessage *FileMetaMessage::dumpMessage(std::string body) {
    auto map = MessageBody::parseMessage(body);

    auto path = map->get("Path");
    if (path.empty()) {
        throw InvalidBodyException(("Invalid body (No Path): " + body).c_str());
    }

    auto sizeString = map->get("Size");
    if (sizeString.empty()) {
        throw InvalidBodyException(("Invalid body (No Size): " + body).c_str());
    }

    long size;
    try {
        size = std::stol(sizeString);
    } catch(std::invalid_argument& e) {
        throw InvalidBodyException(("Invalid body (Size is not number): " + sizeString).c_str());
    }

    return new FileMetaMessage(path, size);
}

FileMeta *FileMetaMessage::getFileMeta() {
    return new FileMeta{path, size};
}

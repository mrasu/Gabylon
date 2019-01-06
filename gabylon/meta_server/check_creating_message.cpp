#include "check_creating_message.h"
#include "../server_exceptions/invalid_body_exception.h"

CheckCreatingMessage *CheckCreatingMessage::dumpMessage(const std::string &body) {
    auto map = MessageBody::parseMessage(body);

    auto fileId = map->get("FileId");
    if (fileId.empty()) {
        throw InvalidBodyException(("Invalid body (No FileId): " + body).c_str());
    }

    return new CheckCreatingMessage(fileId);
}

CheckCreatingMessage::CheckCreatingMessage(const std::string &fileId): fileId(fileId) {

}

#include "check_creating_message.h"
#include "../server_exceptions/invalid_body_exception.h"

CheckCreatingMessage *CheckCreatingMessage::dumpMessage(const std::string &body) {
    auto map = parseBody(body);

    auto search = map.find("FileId");
    if (search == map.end()) {
        throw InvalidBodyException(("Invalid body (No FileId): " + body).c_str());
    }
    auto fileId = search->second;

    if (fileId.empty()) {
        throw InvalidBodyException(("Invalid body (Empty field exists): " + body).c_str());
    }
    return new CheckCreatingMessage(fileId);
}

CheckCreatingMessage::CheckCreatingMessage(const std::string &fileId): fileId(fileId) {

}

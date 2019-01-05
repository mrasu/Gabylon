#include "message_body.h"
#include "../server_exceptions/invalid_body_exception.h"

std::unordered_map<std::string, std::string> MessageBody::parseBody(const std::string &body) {
    std::unordered_map<std::string, std::string> messageMap;

    std::string key;
    std::string value;
    bool isKeyPart = true;
    for (char ch: body) {
        if (ch == '\n') {
            if (key.length() == 0 && value.length() == 0) {
                throw InvalidBodyException(("Invalid body (empty key&value): " + body).c_str());
            }
            if (messageMap.count(key) > 0) {
                throw InvalidBodyException(("Invalid body (empty key): " + body).c_str());
            }

            messageMap[key] = value;
            key = "";
            value = "";
            isKeyPart = true;
        } else if (ch == ':') {
            isKeyPart = false;
        } else {
            if (isKeyPart) {
                key += ch;
            } else {
                value += ch;
            }
        }
    }

    if (key.length() != 0 && value.length() != 0) {
        messageMap[key] = value;
    } else if (key.length() != 0 || value.length() != 0) {
        throw InvalidBodyException(("Invalid body (empty key): " + body).c_str());
    }

    return messageMap;
}

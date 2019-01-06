#include "message_body.h"
#include "../server_exceptions/invalid_body_exception.h"

MessageBody *MessageBody::parseMessage(std::string body) {
    std::unordered_map<std::string, std::string> parsedMap;

    std::string key;
    std::string value;
    bool isKeyPart = true;
    for (char ch: body) {
        if (ch == '\n') {
            if (key.length() == 0 && value.length() == 0) {
                throw InvalidBodyException(("Invalid body (empty key&value): " + body).c_str());
            }
            if (parsedMap.count(key) > 0) {
                throw InvalidBodyException(("Invalid body (empty key): " + body).c_str());
            }

            parsedMap[key] = value;
            key = "";
            value = "";
            isKeyPart = true;
        } else if (ch == ':' && isKeyPart) {
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
        parsedMap[key] = value;
    } else if (key.length() != 0 || value.length() != 0) {
        throw InvalidBodyException(("Invalid body (empty key): " + body).c_str());
    }

    return new MessageBody(parsedMap);
}

MessageBody::MessageBody(const std::unordered_map<std::string, std::string> map): map(map) {}

std::string MessageBody::get(std::string key) {
    auto search = map.find(key);
    if (search == map.end()) {
        return "";
    }
    return search->second;
}

long MessageBody::getLong(const std::string key) {
    auto keyText = get(key);
    if (keyText.empty()) {
        return -1;
    }
    try {
        return std::stol(keyText);
    } catch(std::invalid_argument&) {
        return -1;
    }
}


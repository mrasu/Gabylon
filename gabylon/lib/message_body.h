#ifndef CLIENT_MESSAGE_BODY_H
#define CLIENT_MESSAGE_BODY_H


#include <string>
#include <unordered_map>

class MessageBody {
protected:
    static std::unordered_map<std::string, std::string> parseBody(const std::string &body);
};


#endif //CLIENT_MESSAGE_BODY_H

#ifndef CLIENT_MESSAGE_BODY_H
#define CLIENT_MESSAGE_BODY_H


#include <string>
#include <unordered_map>

class MessageBody {
private:
    std::unordered_map<std::string, std::string> map;
    explicit MessageBody(std::unordered_map<std::string, std::string> map);
public:
    static MessageBody* parseMessage(std::string);
    std::string get(std::string);
    long getLong(std::string);
};


#endif //CLIENT_MESSAGE_BODY_H

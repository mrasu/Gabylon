#ifndef CLIENT_META_MESSAGE_H
#define CLIENT_META_MESSAGE_H

#include <string>

enum MetaMessageMethod {
    CREATE = 0,
    SAVE = 1
};

class MetaMessage {
public:
    static MetaMessage* dumpText(std::string);
    MetaMessageMethod method;
    std::string body;
};


#endif //CLIENT_META_MESSAGE_H

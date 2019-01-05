#ifndef CLIENT_CONTENT_MESSAGE_H
#define CLIENT_CONTENT_MESSAGE_H

#include <string>
#include "content_message_method.h"

class ContentMessage {
public:
    static ContentMessage* dumpText(std::string);
    ContentMessage(ContentMessageMethod, std::string);
    ContentMessageMethod method;
    std::string body;
};


#endif //CLIENT_CONTENT_MESSAGE_H

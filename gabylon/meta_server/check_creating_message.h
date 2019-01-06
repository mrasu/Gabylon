#ifndef CLIENT_CHECK_CREATING_MESSAGE_H
#define CLIENT_CHECK_CREATING_MESSAGE_H


#include "../lib/message_body.h"

class CheckCreatingMessage {
public:
    static CheckCreatingMessage *dumpMessage(const std::string &body);
    explicit CheckCreatingMessage(const std::string &fileId);
    std::string fileId;
};


#endif //CLIENT_CHECK_CREATING_MESSAGE_H

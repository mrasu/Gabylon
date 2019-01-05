#include <cstring>
#include "meta_message.h"
#include "../server_exceptions/invalid_method_exception.h"

MetaMessage* MetaMessage::dumpText(std::string textMessage) {
    std::string methodText;
    std::string body;

    bool bodyStarts = false;
    for (char ch: textMessage) {
        if (bodyStarts) {
            body += ch;
        } else {
            if (ch == '\n') {
                bodyStarts = true;
            } else {
                methodText += ch;
            }
        }
    }

    MetaMessageMethod method;
    if (methodText == "CREATE") {
        method = CREATE;
    } else if (methodText == "SAVE") {
        method = SAVE;
    } else if (methodText == "CHECK_CREATING") {
        method = CHECK_CREATING;
    } else {
        std::string errorMessage = "Invalid method: ";
        throw InvalidMethodException(errorMessage.append(methodText).c_str());
    }

    auto *message = new MetaMessage();
    message->method = method;
    message->body = body;

    return message;
}

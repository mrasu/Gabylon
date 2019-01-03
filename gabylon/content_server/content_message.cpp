#include <utility>

#include "content_message.h"
#include "../exceptions/invalid_method_exception.h"

ContentMessage *ContentMessage::dumpText(std::string textMessage) {
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
            };
        }
    }

    ContentMessageMethod method;
    if (methodText == "WRITE") {
        method = WRITE;
    } else {
        std::string errorMessage = "Invalid method: ";
        throw InvalidMethodException(errorMessage.append(methodText).c_str());
    }

    return new ContentMessage(method, body);
}

ContentMessage::ContentMessage(ContentMessageMethod method, std::string body): method(method), body(std::move(body)) {

}

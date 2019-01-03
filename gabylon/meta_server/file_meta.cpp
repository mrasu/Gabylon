#include <utility>

#include "file_meta.h"
#include "../exceptions/invalid_body_exception.h"

FileMeta::FileMeta(std::string path): path(std::move(path)) {}

FileMeta *FileMeta::dumpMessage(std::string messageBody) {
    std::string path;
    for(char ch:messageBody) {
        if (ch == '\n') {
            if (path.length() > 0) {
                break;
            } else {
                continue;
            }
        }

        path += ch;
    }

    if (path.length() > 0) {
        return new FileMeta(path);
    } else {
        throw InvalidBodyException("Invalid body: path is empty");
    }
}

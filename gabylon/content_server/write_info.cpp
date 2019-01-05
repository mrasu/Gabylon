#include <utility>
#include <unordered_map>

#include "write_info.h"
#include "../server_exceptions/invalid_body_exception.h"

WriteInfo *WriteInfo::dumpMessage(std::string textMessage) {
    std::unordered_map<std::string, std::string> messageMap;

    std::string key;
    std::string value;
    bool isKeyPart = true;
    for (char ch: textMessage) {
       if (ch == '\n') {
           if (key.length() == 0 && value.length() == 0) {
               throw InvalidBodyException(("Invalid body: " + textMessage).c_str());
           }
           if (messageMap.count(key) > 0) {
               throw InvalidBodyException(("Invalid body: " + textMessage).c_str());
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

    auto search = messageMap.find("FileId");
    if (search == messageMap.end()) {
        return nullptr;
    }
    auto fileId = search->second;

    search = messageMap.find("Size");
    if (search == messageMap.end()) {
        return nullptr;
    }
    auto size = std::stol(search->second);

    search = messageMap.find("Path");
    if (search == messageMap.end()) {
        return nullptr;
    }
    auto path = search->second;

    if (!fileId.empty() && size != 0 && !path.empty()) {
        return new WriteInfo(fileId, size, path);
    } else {
        return nullptr;
    }
}

WriteInfo::WriteInfo(const std::string fileId, long size, const std::string path):
    fileId(fileId),
    size(size),
    path(path) {

}


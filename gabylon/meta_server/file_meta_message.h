#ifndef CLIENT_FILE_METADATA_H
#define CLIENT_FILE_METADATA_H


#include <string>
#include "../lib/message_body.h"
#include "file_meta.h"

class FileMetaMessage {
private:
    std::string path;
public:
    static FileMetaMessage* dumpMessage(std::string messageBody);
    explicit FileMetaMessage(std::string path, long size);
    FileMeta *getFileMeta();
    long size;
};


#endif //CLIENT_FILE_METADATA_H

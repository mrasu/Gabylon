#ifndef CLIENT_FILE_DATA_H
#define CLIENT_FILE_DATA_H


#include "file_content.h"
#include "../meta_server/file_meta_message.h"

class FileData {
public:
    FileData(FileMetaMessage *meta, FileContent *content);
    FileContent *content;
    FileMetaMessage *meta;
    int write(std::string basePath);
};


#endif //CLIENT_FILE_DATA_H

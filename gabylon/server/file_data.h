#ifndef CLIENT_FILE_DATA_H
#define CLIENT_FILE_DATA_H


#include "file_content.h"
#include "../meta_server/file_meta.h"

class FileData {
public:
    FileData(FileMeta *meta, FileContent *content);
    FileContent *content;
    FileMeta *meta;
    int write(std::string basePath);
};


#endif //CLIENT_FILE_DATA_H

#ifndef CLIENT_FILE_METADATA_H
#define CLIENT_FILE_METADATA_H


#include <string>

class FileMeta {
public:
    explicit FileMeta(std::string path);
    std::string path;
};


#endif //CLIENT_FILE_METADATA_H

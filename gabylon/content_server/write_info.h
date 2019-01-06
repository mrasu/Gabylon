#ifndef CLIENT_WRITE_INFO_H
#define CLIENT_WRITE_INFO_H


#include <string>

class WriteInfo {
public:
    static WriteInfo *dumpMessage(std::string);
    WriteInfo(std::string, long, std::string, long);
    std::string fileId;
    long size;
    std::string path;
    long from;
};


#endif //CLIENT_WRITE_INFO_H

#include <vector>

#ifndef CLIENT_FILE_CONTENT_H
#define CLIENT_FILE_CONTENT_H


class FileContent {
public:
    explicit FileContent(std::vector<char>);
    std::vector<char> content;
};


#endif //CLIENT_FILE_CONTENT_H

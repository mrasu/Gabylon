#include <fstream>
#include "file_data.h"

FileData::FileData(FileMeta *meta, FileContent *content): meta(meta), content(content) {}

int FileData::write(std::string basePath) {
    std::ofstream outputStream;
    auto content = this->content->content;
    outputStream.open(basePath + this->meta->path, std::ifstream::binary | std::ifstream::out);
    outputStream.write(&content[0], content.size());
    outputStream.close();
}

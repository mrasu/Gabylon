#include <utility>

#include "file_content.h"

FileContent::FileContent(std::vector<char> content): content(std::move(content)) {}

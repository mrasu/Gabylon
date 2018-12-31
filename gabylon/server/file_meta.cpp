#include <utility>

#include "file_meta.h"

FileMeta::FileMeta(std::string path): path(std::move(path)) {}

cmake_minimum_required(VERSION 3.13)
project(filer)
project(client)

set(CMAKE_CXX_STANDARD 17)

add_executable(filer main.cpp gabylon/server/gabylon_server.cpp gabylon/server/gabylon_server.h gabylon/server/file_data.cpp gabylon/server/file_data.h gabylon/server/file_meta.cpp gabylon/server/file_meta.h gabylon/server/file_content.cpp gabylon/server/file_content.h)
add_executable(client client.cpp)

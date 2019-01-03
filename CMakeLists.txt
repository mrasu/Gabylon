cmake_minimum_required(VERSION 3.13)
project(server)
project(client)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS thread)
# add_executable(server meta_server.cpp gabylon/server/gabylon_server.cpp gabylon/server/gabylon_server.h gabylon/server/file_data.cpp gabylon/server/file_data.h gabylon/meta_server/file_meta.cpp gabylon/meta_server/file_meta.h gabylon/server/file_content.cpp gabylon/server/file_content.h)
add_executable(server meta_server.cpp gabylon/meta_server/gabylon_meta_server.h gabylon/meta_server/gabylon_meta_server.cpp gabylon/meta_server/file_meta.cpp gabylon/meta_server/file_meta.h gabylon/server/file_content.cpp gabylon/server/file_content.h gabylon/exceptions/invalid_method_exception.h gabylon/exceptions/invalid_body_exception.cpp gabylon/exceptions/invalid_body_exception.h gabylon/exceptions/invalid_method_exception.cpp gabylon/meta_server/meta_message.h gabylon/meta_server/meta_message.cpp gabylon/content_server/gabylon_content_server.cpp gabylon/content_server/gabylon_content_server.h gabylon/content_server/content_message.cpp gabylon/content_server/content_message.h gabylon/content_server/content_message_method.h gabylon/content_server/write_info.cpp gabylon/content_server/write_info.h)
add_executable(client client.cpp)

include_directories("${Boost_INCLUDE_DIRS}")
target_link_libraries(server Boost::system)

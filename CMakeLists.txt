cmake_minimum_required(VERSION 3.13)
project(server)
project(client)

set(CMAKE_CXX_STANDARD 17)

set(lib_srcs
        gabylon/lib/server_base.cpp
        gabylon/lib/server_base.h
        gabylon/lib/message_body.h
        gabylon/lib/message_body.cpp
        gabylon/lib/socket_read_writer.cpp
        gabylon/lib/socket_read_writer.h
)

set(meta_server_srcs
        gabylon/meta_server/gabylon_meta_server.h
        gabylon/meta_server/gabylon_meta_server.cpp
        gabylon/meta_server/file_meta_message.cpp
        gabylon/meta_server/file_meta_message.h
        gabylon/meta_server/meta_message.h
        gabylon/meta_server/meta_message.cpp
        gabylon/meta_server/check_creating_message.cpp
        gabylon/meta_server/check_creating_message.h
        gabylon/meta_server/file_meta.h
)

set(struct_srcs
        gabylon/structs/content_server_info.h
        gabylon/structs/ContentServerInfo.cpp
)

set(server_exceptions
        gabylon/server_exceptions/invalid_method_exception.h
        gabylon/server_exceptions/invalid_body_exception.cpp
        gabylon/server_exceptions/invalid_body_exception.h
        gabylon/server_exceptions/invalid_method_exception.cpp
)

set(internal_client_srcs
        gabylon/internal_client/gabylon_meta_server_client.cpp
        gabylon/internal_client/gabylon_meta_server_client.h
        gabylon/internal_client/client_exception.cpp
        gabylon/internal_client/client_exception.h
)

find_package(Boost REQUIRED COMPONENTS thread)
add_executable(meta_server
        meta_server.cpp
        ${lib_srcs}
        ${meta_server_srcs}
        ${struct_srcs}
        ${server_exceptions}
        gabylon/server/file_content.cpp
        gabylon/server/file_content.h
)

set(content_server_srcs
        gabylon/content_server/gabylon_content_server.cpp
        gabylon/content_server/gabylon_content_server.h
        gabylon/content_server/content_message.cpp
        gabylon/content_server/content_message.h
        gabylon/content_server/content_message_method.h
        gabylon/content_server/write_info.cpp
        gabylon/content_server/write_info.h
)

add_executable(content_server
        content_server.cpp
        ${lib_srcs}
        ${server_exceptions}
        ${internal_client_srcs}
        ${content_server_srcs}
        gabylon/server/file_content.cpp
        gabylon/server/file_content.h)


add_executable(client client.cpp
        ${lib_srcs}
        ${server_exceptions}
)

include_directories("${Boost_INCLUDE_DIRS}")
target_link_libraries(meta_server Boost::system)
target_link_libraries(content_server Boost::system)

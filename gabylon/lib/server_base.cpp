#include <sys/socket.h>
#include <cstring>
#include <vector>
#include <zconf.h>
#include "server_base.h"

std::string ServerBase::readSentData(int socket) {
    char buffer[50];
    memset(buffer, 0, sizeof(buffer));

    std::vector<char> messages;
    bool is_previous_new_line = false;
    bool reaches_end = false;
    while(true) {
        auto ret = read(socket, buffer, sizeof(buffer));

        if (ret < 0) {
            printf("read error!: %zd", ret);
            return nullptr;
        } else if (ret == 0) {
            break;
        }

        for (int i = 0; i < ret; i++) {
            if (buffer[i] == '\n') {
                if (is_previous_new_line) {
                    reaches_end = true;
                    break;
                }
                is_previous_new_line = true;
            } else if (is_previous_new_line){
                is_previous_new_line = false;
            }
            messages.push_back(buffer[i]);
        }
        if (reaches_end) {
            break;
        }
    }

    while(!messages.empty() && messages.back() == '\n') {
        messages.pop_back();
    }

    if (messages.empty()) {
        return "";
    } else {
        return std::string(messages.begin(), messages.end());
    }
}

ssize_t ServerBase::error(int socket, const std::string &humanErrorMessage) {
    std::string message = "ERROR: " + humanErrorMessage + "\n\n";
    return reply(socket, message);
}

ssize_t ServerBase::reply(int socket, const std::string &message) {
    auto ret = send(socket, message.c_str(), message.length(), 0);
    if (ret < 0) {
        return ret;
    } else {
        return 0;
    }
}

ssize_t ServerBase::ok(int socket) {
    return reply(socket, "OK");
}

ssize_t ServerBase::ng(int socket) {
    return reply(socket, "NG");
}

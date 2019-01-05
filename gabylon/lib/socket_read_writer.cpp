#include <cstring>
#include <vector>
#include <zconf.h>
#include "socket_read_writer.h"

SocketReadWriter::SocketReadWriter(int socket): socketFileDescriptor(socket) {

}

std::string SocketReadWriter::readReceivedText() {
    char buffer[50];
    memset(buffer, 0, sizeof(buffer));

    std::vector<char> messages;
    bool is_previous_new_line = false;
    bool reaches_end = false;
    while(true) {
        auto ret = read(socketFileDescriptor, buffer, sizeof(buffer));

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

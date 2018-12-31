#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <zconf.h>
#include <arpa/inet.h>

int main() {
    std::cout << "client!" << std::endl;
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server = {};
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    // std::string in_file_path = "test_short.txt";
    // std::string in_file_path = "test_long.txt";
    std::string in_file_path = "Crown.png";
    std::ifstream is("./input/" + in_file_path, std::ifstream::binary);
    if (!is) {
        printf("Failed to create stream");
        return 1;
    }

    ssize_t ret;
    std::string file_path = "/" + in_file_path + "\n";
    ret = send(sock, file_path.c_str(), file_path.length(), 0);
    if (ret < 0) {
        printf("Failed to send file path!: %zd", ret);
        return 1;
    }

    char buffer[1000];
    std::string text;
    while(!is.eof()) {
        memset(buffer, 0, sizeof(buffer));

        is.read(buffer, sizeof(buffer));
        ret = send(sock, buffer, (size_t)is.gcount(), 0);
        if (ret < 0) {
            printf("Failed to send!: %zd", ret);
            break;
        }

        printf("sending...\n");
    }
    is.close();

    printf("FIN!\n");

    close(sock);

    return 0;
}

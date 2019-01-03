#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <zconf.h>
#include <arpa/inet.h>

int send_meta();

int main() {
    std::cout << "client!" << std::endl;
    /*
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
        printf("Failed to dumpMessage stream");
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
     */
    return send_meta();

    return 0;
}

int send_meta() {
    printf("sending meta...\n");
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    // setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    // setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    struct sockaddr_in server = {};
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    // std::string in_file_path = "test_short.txt";
    // std::string in_file_path = "test_long.txt";
    std::string in_file_path = "Crown.png";

    ssize_t ret;
    std::string message = "CREATE\n/" + in_file_path + "\n\n";
    ret = send(sock, message.c_str(), message.length(), 0);
    if (ret < 0) {
        printf("Failed to send file path!: %zd", ret);
        return 1;
    }
    printf("Sent path!\n");

    char buffer[100];
    memset(buffer, 0, sizeof(buffer));

    std::string writing_id;
    while(true) {
        ret = read(sock, buffer, sizeof(buffer));
        if (ret < 0) {
            printf("Failed to read answer: %zd", ret);
            return 1;
        } else if (ret == 0) {
            break;
        }

        for (int i = 0; i < ret; i++) {
            writing_id += buffer[i];
        }
    }

    printf("FIN! (id: %s)\n", writing_id.c_str());

    close(sock);

    return 0;
}

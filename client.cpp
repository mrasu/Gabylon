#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <zconf.h>
#include <arpa/inet.h>

std::string sendMeta(const std::string &in_file_path);
int sendData(const std::string&, const std::string &in_file_path);

int main() {
    std::cout << "Start client!" << std::endl;

    // std::string in_file_path = "test_short.txt";
    // std::string in_file_path = "test_long.txt";
    std::string in_file_path = "Crown.png";

    auto fileId = sendMeta(in_file_path);
    if (fileId.empty()) {
        return 1;
    }
    return sendData(fileId, in_file_path);
}

std::string sendMeta(const std::string &in_file_path) {
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

    ssize_t ret;
    std::string message = "CREATE\n/" + in_file_path + "\n\n";
    ret = send(sock, message.c_str(), message.length(), 0);
    if (ret < 0) {
        printf("Failed to send file path!: %zd", ret);
        return "";
    }
    printf("Sent path!\n");

    char buffer[100];
    memset(buffer, 0, sizeof(buffer));

    std::string writing_id;
    while(true) {
        ret = read(sock, buffer, sizeof(buffer));
        if (ret < 0) {
            printf("Failed to read answer: %zd", ret);
            return "";
        } else if (ret == 0) {
            break;
        }

        for (int i = 0; i < ret; i++) {
            writing_id += buffer[i];
        }
    }

    printf("FIN! (id: %s)\n", writing_id.c_str());

    close(sock);

    return writing_id;
}

int sendData(const std::string &fileId, const std::string &in_file_path) {
    printf("sending data...\n");
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server = {};
    server.sin_family = AF_INET;
    server.sin_port = htons(12346);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    std::ifstream is("./input/" + in_file_path, std::ifstream::binary);
    if (!is) {
        printf("Failed to dumpMessage stream");
        return 1;
    }

    is.seekg(0, std::fstream::end);
    const auto end = is.tellg();
    is.clear();
    is.seekg(0, std::fstream::beg);
    const auto start = is.tellg();
    const auto size = end - start;

    ssize_t ret;
    std::string message = "WRITE\nFileId:" + fileId + "\nSize:" + std::to_string(size) + "\nPath:"+in_file_path+"\n\n";
    ret = send(sock, message.c_str(), message.length(), 0);
    if (ret < 0) {
        printf("Failed to send file path!: %zd", ret);
        return 1;
    }

    char buffer[1000];
    ret = read(sock, buffer, sizeof(buffer));
    if (ret < 0) {
        printf("Failed to read: %zd", ret);
        return 1;
    }
    printf("%s\n", buffer);

    char fileBuffer[1000];
    memset(fileBuffer, 0, sizeof(fileBuffer));
    std::string text;
    while(!is.eof()) {
        is.read(fileBuffer, sizeof(fileBuffer));
        ret = send(sock, fileBuffer, (size_t)is.gcount(), 0);
        if (ret < 0) {
            printf("Failed to send!: %zd", ret);
            is.close();
            return -1;
        }

        printf("sending...\n");
    }

    memset(buffer, 0, sizeof(buffer));
    ret = read(sock, buffer, sizeof(buffer));
    if (ret < 0) {
        printf("Failed to read: %zd", ret);
        return 1;
    }
    printf("%s\n", buffer);

    is.close();

    printf("FIN!\n");

    close(sock);
}

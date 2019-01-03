#include <iostream>
#include <netinet/in.h>
#include <zconf.h>
#include <cstring>
#include <arpa/inet.h>
#include <bits/ios_base.h>
#include <fstream>
#include <array>
#include <vector>
#include "gabylon/server/gabylon_server.h"
#include "gabylon/meta_server/gabylon_meta_server.h"
#include "gabylon/content_server/gabylon_content_server.h"

int main() {
    std::cout << "main" << std::endl;
    // auto *server = new GabylonServer("./output/");
    // auto *server = new GabylonMetaServer();

    struct sockaddr_in metaServerAddr = {};
    metaServerAddr.sin_family = AF_INET;
    metaServerAddr.sin_port = htons(12345);
    metaServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    auto *server =  new GabylonContentServer("./output/", metaServerAddr);
    return server->start();
}

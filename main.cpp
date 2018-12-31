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

int main() {
    std::cout << "main" << std::endl;
    auto *server = new GabylonServer("./output/");
    server->start();
    return 0;
}

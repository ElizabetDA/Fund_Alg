#include "../include/Client.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

Client::Client(const std::string& address, uint16_t port)
  : sock_(AF_INET, SOCK_STREAM, 0)
{
    sock_.connect(address, port);
}

void Client::run() {
    std::string cmd;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, cmd)) break;
        if (cmd == "quit" || cmd == "exit") break;

        // framing: длина + данные

        uint32_t len = cmd.size();
        sock_.send(&len, sizeof(len));
        sock_.send(cmd.data(), len);

        // получить ответ
        sock_.recv(&len, sizeof(len));
        std::vector<char> buf(len);
        sock_.recv(buf.data(), len);
        std::cout << std::string(buf.begin(), buf.end()) << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: client <address> <port>\n";
        return 1;
    }
    try {
        Client c(argv[1], static_cast<uint16_t>(std::stoi(argv[2])));
        c.run();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

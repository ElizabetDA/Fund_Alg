#include "../include/Socket.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>
#include <cstdio>  // for sscanf

static void sendCommand(Socket& sock, const std::string& cmd) {
    uint32_t len = cmd.size();
    sock.send(&len, sizeof(len));
    sock.send(cmd.data(), len);
}

static std::string recvReply(Socket& sock) {
    uint32_t len;
    sock.recv(&len, sizeof(len));
    std::vector<char> buf(len);
    sock.recv(buf.data(), len);
    return {buf.begin(), buf.end()};
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: client <address> <port>\n";
        return 1;
    }

    try {
        Socket sock(AF_INET, SOCK_STREAM, 0);
        sock.connect(argv[1], static_cast<uint16_t>(std::stoi(argv[2])));

        while (true) {
            std::cout << "\n=== MENU ===\n"
                      << "1) Compile .cpp/.tex\n"
                      << "2) Play sticks game\n"
                      << "3) Exit\n"
                      << "Select [1-3]: ";
            int choice;
            if (!(std::cin >> choice))
                break;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice == 1) {
                // Сборка файла
                std::cout << "Enter filename (with .cpp or .tex): ";
                std::string fn;
                std::getline(std::cin, fn);
                sendCommand(sock, "compile " + fn);
                std::string reply = recvReply(sock);
                std::cout << reply << "\n";

            } else if (choice == 2) {
                // Игра "Палочки"
                const int INITIAL = 21;
                int remaining = INITIAL;
                std::cout << "Starting sticks game (total " << INITIAL << ").\n";

                while (true) {
                    std::cout << "\nSticks left: " << remaining << "\n"
                              << "Your move (take 1-3): ";
                    int take;
                    if (!(std::cin >> take)) {
                        // некорректный ввод — выходим из игры
                        break;
                    }
                    if (take < 1 || take > 3 || take > remaining) {
                        std::cout << "Invalid move — must take 1 to min(3," << remaining << ")\n";
                        continue;
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    sendCommand(sock, "game " + std::to_string(take));
                    std::string resp = recvReply(sock);

                    // 1) Конец игры (вы выиграли)
                    if (resp.rfind("Game over", 0) == 0) {
                        std::cout << "\n=== GAME OVER ===\n"
                                  << resp << "\n";
                        break;
                    }
                    // 2) Некорректный ход (сервер вернул ошибку)
                    if (resp.rfind("Error:", 0) == 0) {
                        std::cout << resp << "\n";
                        continue;
                    }
                    // 3) Обычный ход сервера
                    int srv = 0;
                    if (std::sscanf(resp.c_str(), "Server took %d", &srv) != 1) {
                        std::cout << "Unexpected response: " << resp << "\n";
                        break;
                    }
                    remaining -= (take + srv);
                    std::cout << "Server took " << srv << ".\n";

                    // 4) Если палочки закончились после его хода — сервер выиграл
                    if (remaining <= 0) {
                        std::cout << "\n=== GAME OVER ===\n"
                                  << "Server wins. 💥\n";
                        break;
                    }
                }

            } else if (choice == 3) {
                // Выход
                sendCommand(sock, "quit");
                std::cout << "Goodbye!\n";
                break;

            } else {
                std::cout << "Invalid menu choice\n";
            }
        }

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}

#include "../include/Server.hpp"
#include <cstring>
#include <functional>
#include <unordered_map>

Server::Server(uint16_t port)
  : sock_(AF_INET, SOCK_STREAM, 0),
    shm_(ftok(".", 'S'), sizeof(CompileTask)),
    mq_(ftok(".", 'M'))
{
    sock_.bind("0.0.0.0", port);
    sock_.listen();
    Logger::info("Server listening on port {}", port);
}

void Server::run() {
    while (true) {
        auto client = sock_.accept();
        Logger::info("New client connected");
        std::thread([this, c = std::move(client)]() mutable {
            handleClient(*c);
        }).detach();
    }
}

void Server::handleClient(Socket clientSock) {
    try {
        while (true) {
            uint32_t len;
            clientSock.recv(&len, sizeof(len));
            std::vector<char> buf(len);
            clientSock.recv(buf.data(), len);
            std::string cmd(buf.begin(), buf.end());
            Logger::info("Received: {}", cmd);

            // --- БЛОК КОМПИЛЯЦИИ ---
            if (cmd.rfind("compile ", 0) == 0) {
                std::string fn = cmd.substr(8);
                CompileTask t{};
                t.client_id = static_cast<int>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
                std::strncpy(t.filename, fn.c_str(), sizeof(t.filename)-1);
                shm_.write(t);         // отдали задачу
                shm_.read();           // ждём, пока вернут
                std::string reply = "Compiled " + fn;
                uint32_t rl = reply.size();
                clientSock.send(&rl, sizeof(rl));
                clientSock.send(reply.data(), rl);

            // --- БЛОК ИГРЫ ---
            } else if (cmd.rfind("game ", 0) == 0) {
                int take = std::stoi(cmd.substr(5));

                // 1) валидация входного хода
                if (take < 1 || take > 3) {
                    std::string err = "Error: invalid move (1-3 only)";
                    uint32_t l = err.size();
                    clientSock.send(&l, sizeof(l));
                    clientSock.send(err.data(), l);
                    continue;
                }

                // 2) отправляем ход в очередь mtype=1
                GameMsg out{ 1, take };
                mq_.send(out);

                // 3) ждём ответ от подсерверa (тот же mtype)
                GameMsg in = mq_.recv(1);

                // 4) формируем ответ по трём состояниям
                std::string reply;
                if (in.take < 0) {
                    // защита от внутренних ошибок
                    reply = "Error: invalid move (1-3 only)";
                }
                else if (in.take == 0) {
                    // сигнал, что игра закончилась и клиент выиграл
                    reply = "Game over — you win!";
                }
                else {
                    // обычный ход сервера
                    reply = "Server took " + std::to_string(in.take);
                }

                uint32_t rl = reply.size();
                clientSock.send(&rl, sizeof(rl));
                clientSock.send(reply.data(), rl);

            // --- Завершение сессии ---
            } else if (cmd == "quit") {
                break;

            // --- Неизвестная команда ---
            } else {
                std::string reply = "Unknown command";
                uint32_t rl = reply.size();
                clientSock.send(&rl, sizeof(rl));
                clientSock.send(reply.data(), rl);
            }
        }
    } catch (const std::exception& ex) {
        Logger::error("Client handler exception: {}", ex.what());
    }
    Logger::info("Client disconnected");
}

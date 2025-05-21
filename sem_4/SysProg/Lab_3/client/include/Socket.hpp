#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <memory>

class Socket {
public:
    Socket(int domain, int type, int protocol) {
        fd_ = ::socket(domain, type, protocol);
        if (fd_ == -1)
            throw std::runtime_error(std::string("socket failed: ") + std::strerror(errno));
    }
    ~Socket() {
        if (fd_ != -1) ::close(fd_);
    }

    // Конструктор для принятого fd
    explicit Socket(int fd): fd_(fd) {}

    void bind(const std::string& addr, uint16_t port) {
        sockaddr_in sa{};
        sa.sin_family = AF_INET;
        sa.sin_port = htons(port);
        if (::inet_pton(AF_INET, addr.c_str(), &sa.sin_addr) <= 0)
            throw std::runtime_error("inet_pton failed");
        if (::bind(fd_, (sockaddr*)&sa, sizeof(sa)) == -1)
            throw std::runtime_error(std::string("bind failed: ") + std::strerror(errno));
    }

    void listen(int backlog = 5) {
        if (::listen(fd_, backlog) == -1)
            throw std::runtime_error(std::string("listen failed: ") + std::strerror(errno));
    }

    std::unique_ptr<Socket> accept() {
        int client_fd = ::accept(fd_, nullptr, nullptr);
        if (client_fd == -1)
            throw std::runtime_error(std::string("accept failed: ") + std::strerror(errno));
        return std::make_unique<Socket>(client_fd);
    }

    void connect(const std::string& addr, uint16_t port) {
        sockaddr_in sa{};
        sa.sin_family = AF_INET;
        sa.sin_port = htons(port);
        if (::inet_pton(AF_INET, addr.c_str(), &sa.sin_addr) <= 0)
            throw std::runtime_error("inet_pton failed");
        if (::connect(fd_, (sockaddr*)&sa, sizeof(sa)) == -1)
            throw std::runtime_error(std::string("connect failed: ") + std::strerror(errno));
    }

    ssize_t send(const void* buf, size_t len, int flags = 0) {
        ssize_t res = ::send(fd_, buf, len, flags);
        if (res == -1)
            throw std::runtime_error(std::string("send failed: ") + std::strerror(errno));
        return res;
    }

    ssize_t recv(void* buf, size_t len, int flags = 0) {
        ssize_t res = ::recv(fd_, buf, len, flags);
        if (res == -1)
            throw std::runtime_error(std::string("recv failed: ") + std::strerror(errno));
        return res;
    }

private:
    int fd_;
};

#endif // SOCKET_HPP

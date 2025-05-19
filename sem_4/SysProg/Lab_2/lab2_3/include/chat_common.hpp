#pragma once

#include <cstddef>
#include <sys/types.h>

// Размеры буфера и текста сообщения
constexpr std::size_t CHAT_BUF_SIZE  = 1024;
constexpr std::size_t CHAT_TEXT_SIZE = 256;

// Одно сообщение в чате
struct Message {
	int  from_id;
	char text[CHAT_TEXT_SIZE];
};

// Заголовок в начале SHM:
//   client_count — число зарегистрированных клиентов
//   write_index  — глобальный счётчик сообщений
//   далее идут client_count пар (client_id, read_index)
//   потом массив Message messages[CHAT_BUF_SIZE]
struct ShmHeader {
	int client_count;
	int write_index;
	// далее: пары (client_id, read_index) × client_count
	// потом: Message messages[CHAT_BUF_SIZE]
};

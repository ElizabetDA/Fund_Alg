#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdbool.h>

/*
 * В задаче "Волк, коза, капуста" у нас есть три объекта (wolf, goat, cabbage).
 * Крестьянин и объекты могут находиться на разных берегах:
 *   LEFT  = 0,
 *   BOAT  = 1,
 *   RIGHT = 2.
 *
 * Ниже – простая модель, описывающая текущее состояние игры.
 */

// Максимальный размер текстового сообщения
#define MAX_TEXT 128

// Коды типов сообщений: от клиента к серверу и от сервера к клиенту
#define MSG_TYPE_REQUEST  1
#define MSG_TYPE_RESPONSE 2

// Идентификаторы объектов
typedef enum {
	OBJ_WOLF = 0,
	OBJ_GOAT,
	OBJ_CABBAGE,
	OBJ_COUNT // вспомогательное для циклов
} ObjectType;

typedef enum {
	SIDE_LEFT = 0,
	SIDE_BOAT,
	SIDE_RIGHT
} Side;

// Структура для хранения состояния (где находятся волк, коза, капуста, крестьянин)
typedef struct {
	Side wolfSide;
	Side goatSide;
	Side cabbageSide;
	Side farmerSide;
} GameState;

// Формат сообщения для очереди
typedef struct {
	long mtype;                // Тип сообщения (MSG_TYPE_REQUEST или MSG_TYPE_RESPONSE)
	int  clientId;             // Уникальный ID клиента (для многопользовательской работы)
	char text[MAX_TEXT];       // Строка команды или ответ
} Message;

// Проверка, что состояние безопасно (волк не съел козу, коза не съела капусту)
static inline bool isSafe(const GameState *state) {
	// Если крестьянин не на том же берегу, где волк и коза вместе, то волк съест козу
	if (state->wolfSide == state->goatSide && state->wolfSide != state->farmerSide) {
		return false;
	}
	// Если крестьянин не на том же берегу, где коза и капуста вместе, то коза съест капусту
	if (state->goatSide == state->cabbageSide && state->goatSide != state->farmerSide) {
		return false;
	}
	return true;
}

#endif // COMMON_H

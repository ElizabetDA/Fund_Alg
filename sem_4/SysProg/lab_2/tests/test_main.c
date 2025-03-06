#include <stdio.h>
#include "file_operations.h"

//для операции xorN
void test_xorN_operation() {
	char *files[] = {"test_files/test1.txt", "test_files/test2.txt"};
	printf("Testing xorN_operation...\n");
	xorN_operation(2, files, 3); // Пример для N=3
	printf("xorN_operation test passed.\n");
}

//для операции mask
void test_mask_operation() {
	char *files[] = {"test_files/test1.txt", "test_files/test2.txt"};
	printf("Testing mask_operation...\n");
	mask_operation(2, files, 0xFF); // Пример маски 0xFF
	printf("mask_operation test passed.\n");
}

//для операции copyN
void test_copyN_operation() {
	char *files[] = {"test_files/test1.txt"};
	printf("Testing copyN_operation...\n");
	copyN_operation(1, files, 2); // Создание 2 копий
	printf("copyN_operation test passed.\n");
}

// для операции find
void test_find_operation() {
	char *files[] = {"test_files/test1.txt", "test_files/test2.txt"};
	printf("Testing find_operation...\n");
	find_operation(2, files, "hello"); // Поиск строки "hello"
	printf("find_operation test passed.\n");
}

int main() {
	printf("Running tests...\n");

	test_xorN_operation();
	test_mask_operation();
	test_copyN_operation();
	test_find_operation();

	printf("All tests passed.\n");
	return 0;
}
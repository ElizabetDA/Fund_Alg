#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdint.h>

void xorN_operation(int file_count, char *files[], int N);
void mask_operation(int file_count, char *files[], uint32_t mask);
void copyN_operation(int file_count, char *files[], int N);
void find_operation(int file_count, char *files[], const char *search_string);

#endif // FILE_OPERATIONS_H
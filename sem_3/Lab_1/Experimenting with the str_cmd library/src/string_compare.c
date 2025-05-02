#include "../include/string_compare.h"

int string_compare(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return 0; // строки не равны
        }
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0'); // строки равны
}
#include <stdio.h>
#include <stdlib.h>

int is_alpha(char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

int is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

int is_space(char ch) {
    return ch == ' ';
}

void process_d(FILE *input, FILE *output) {
    char ch;
    while ((ch = fgetc(input)) != EOF) {
        if (!is_digit(ch)) {
            fputc(ch, output);
        }
    }
}

void process_i(FILE *input, FILE *output) {
    char ch;
    int count = 0;
    while ((ch = fgetc(input)) != EOF) {
        if (ch == '\n') {
            fprintf(output, "%d\n", count);
            count = 0;
        } else if (is_alpha(ch)) {
            count++;
        }
    }
    if (count > 0) {
        fprintf(output, "%d\n", count);
    }
}

void process_s(FILE *input, FILE *output) {
    char ch;
    int count = 0;
    while ((ch = fgetc(input)) != EOF) {
        if (ch == '\n') {
            fprintf(output, "%d\n", count);
            count = 0;
        } else if (!is_alpha(ch) && !is_digit(ch) && !is_space(ch)) {
            count++;
        }
    }
    if (count > 0) {
        fprintf(output, "%d\n", count);
    }
}

void process_a(FILE *input, FILE *output) {
    char ch;
    while ((ch = fgetc(input)) != EOF) {
        if (!is_digit(ch)) {
            fprintf(output, "%X", (unsigned char)ch);
        } else {
            fputc(ch, output);
        }
    }
}

int str_len(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <flag> <input_file> [output_file]\n", argv[0]);
        return 1;
    }

    char *flag = argv[1];
    char *input_file = argv[2];
    char *output_file = NULL;

    if (flag[1] == 'n') {
        if (argc < 4) {
            int input_file_len = str_len(input_file);
            output_file = (char *)malloc(6 + input_file_len + 1);
            if (output_file == NULL) {
                printf("Error: Cannot allocate memory for output file name\n");
                return 1;
            }
            // Формируем имя выходного файла
            output_file[0] = 'o';
            output_file[1] = 'u';
            output_file[2] = 't';
            output_file[3] = '_';
            for (int i = 0; i < input_file_len; i++) {
                output_file[4 + i] = input_file[i];
            }
            output_file[4 + input_file_len] = '\0';  // Завершение строки
        } else {
            output_file = argv[3];
        }
    } else {
        int input_file_len = str_len(input_file);
        output_file = (char *)malloc(6 + input_file_len + 1);
        if (output_file == NULL) {
            printf("Error: Cannot allocate memory for output file name\n");
            return 1;
        }
        // Формируем имя выходного файла
        output_file[0] = 'o';
        output_file[1] = 'u';
        output_file[2] = 't';
        output_file[3] = '_';
        for (int i = 0; i < input_file_len; i++) {
            output_file[4 + i] = input_file[i];
        }
        output_file[4 + input_file_len] = '\0';  // Завершение строки
    }

    FILE *input = fopen(input_file, "r");
    if (!input) {
        printf("Error: Cannot open input file %s\n", input_file);
        return 1;
    }

    FILE *output = fopen(output_file, "w");
    if (!output) {
        printf("Error: Cannot open or create output file %s\n", output_file);
        fclose(input);
        return 1;
    }

    if (flag[1] == 'd') {
        process_d(input, output);
    } else if (flag[1] == 'i') {
        process_i(input, output);
    } else if (flag[1] == 's') {
        process_s(input, output);
    } else if (flag[1] == 'a') {
        process_a(input, output);
    } else {
        printf("Error: Unknown flag %s\n", flag);
    }

    fclose(input);
    fclose(output);

    free(output_file);

   /* if (flag[1] != 'n' || argc < 4) {
        free(output_file);
    }*/

    return 0;
}

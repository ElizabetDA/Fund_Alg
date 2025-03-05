#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_operations.h"

void print_usage() {
	printf("Usage: ./file_processor <file1> <file2> ... <flag> <args>\n");
	printf("Flags:\n");
	printf("  xorN - XOR operation with block size 2^N (N=2,3,4,5,6)\n");
	printf("  mask <hex> - count 4-byte integers matching the mask\n");
	printf("  copyN - create N copies of each file\n");
	printf("  find <SomeString> - search for a string in the files\n");
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		print_usage();
		return 1;
	}

	char *flag = argv[argc - 2];
	if (strcmp(flag, "xorN") == 0) {
		int N = atoi(argv[argc - 1]);
		if (N < 2 || N > 6) {
			printf("Invalid N for xorN operation. N must be between 2 and 6.\n");
			return 1;
		}
		xorN_operation(argc - 2, argv, N);
	} else if (strcmp(flag, "mask") == 0) {
		if (argc < 4) {
			printf("Missing mask argument.\n");
			return 1;
		}
		uint32_t mask = strtoul(argv[argc - 1], NULL, 16);
		mask_operation(argc - 3, argv, mask);
	} else if (strcmp(flag, "copyN") == 0) {
		int N = atoi(argv[argc - 1]);
		if (N <= 0) {
			printf("Invalid N for copyN operation. N must be positive.\n");
			return 1;
		}
		copyN_operation(argc - 2, argv, N);
	} else if (strcmp(flag, "find") == 0) {
		if (argc < 4) {
			printf("Missing search string argument.\n");
			return 1;
		}
		char *search_string = argv[argc - 1];
		find_operation(argc - 3, argv, search_string);
	} else {
		printf("Unknown flag: %s\n", flag);
		print_usage();
		return 1;
	}

	return 0;
}
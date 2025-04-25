#include "app.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <absolute_path1> [absolute_path2 ...]\n", argv[0]);
		return EXIT_FAILURE;
	}

	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] != '/') {
			fprintf(stderr, "Error: Path '%s' is not absolute.\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	const char *in_fifo = "/tmp/server_input.fifo";
	const char *out_fifo = "/tmp/server_output.fifo";

	int in_fd = open(in_fifo, O_WRONLY);
	if (in_fd == -1) { perror("open in_fifo"); return EXIT_FAILURE; }

	for (int i = 1; i < argc; ++i) {
		// Замена dprintf на write и snprintf
		char buffer[1024];
		int len = snprintf(buffer, sizeof(buffer), "%s\n", argv[i]);
		if (len < 0) {
			perror("snprintf");
			close(in_fd);
			return EXIT_FAILURE;
		}
		ssize_t written = write(in_fd, buffer, len);
		if (written != len) {
			perror("write");
			close(in_fd);
			return EXIT_FAILURE;
		}
	}
	close(in_fd);

	int out_fd = open(out_fifo, O_RDONLY);
	if (out_fd == -1) { perror("open out_fifo"); return EXIT_FAILURE; }

	char buffer[1024];
	ssize_t bytes_read;
	while ((bytes_read = read(out_fd, buffer, sizeof(buffer))) > 0) {
		write(STDOUT_FILENO, buffer, bytes_read);
	}
	close(out_fd);

	return EXIT_SUCCESS;
}
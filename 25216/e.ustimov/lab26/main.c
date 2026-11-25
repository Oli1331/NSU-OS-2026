#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 1024 * 4

int main(int argc, char **argv) {

	if (argc < 2) {
		FILE *pipe_from_child = popen("./app --child", "r");
		if (pipe_from_child == NULL) {
			perror("ошибка открытия трубы через popen");
			return 1;
		}
		char *buf = malloc(sizeof(char) * BUF_SIZE);
		if (buf == NULL) {
			perror("ошибка выделения памяти через malloc");
			return 1;
		}
		size_t n;
		while ((n = fread(buf, sizeof(char), BUF_SIZE, pipe_from_child)) > 0) {
			fwrite(buf, sizeof(char), n, stdout);
		}
		pclose(pipe_from_child);
		free(buf);
	} else if (strcmp(argv[1], "--child") == 0) {
		FILE *text = fopen("main.c", "r");
		if (text == NULL) {
			perror("ошибка открытия файла через fopen");
			return 1;
		}
		char *buf = malloc(sizeof(char) * BUF_SIZE);
		if (buf == NULL) {
			perror("ошибка выделения памяти через malloc");
			return 1;
		}
		size_t n;
		while ((n = fread(buf, sizeof(char), BUF_SIZE, text)) > 0) {
			for (size_t i = 0; i < n; i++) {
				fputc(toupper(buf[i]), stdout);
			}
		}
		fclose(text);
		free(buf);
	}
	return 0;
}
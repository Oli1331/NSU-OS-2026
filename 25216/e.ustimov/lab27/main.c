#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUF_SIZE 1024 * 4

int main(int argc, char** argv) {

	if (argc >= 2) {
		FILE* text = fopen(argv[1], "r");
		if (text == NULL) {
			perror("ошибка открытия файла через fopen");
		}
		else {
			dup2(fileno(text), STDIN_FILENO);
		}
	}
	FILE* pipe_to_child = popen("wc -l", "w");
	if (pipe_to_child == NULL) {
		perror("ошибка открытия трубы через popen");
		return 1;
	}

	char* buf = malloc(sizeof(char) * BUF_SIZE);
	if (buf == NULL) {
		perror("ошибка выделения памяти через malloc");
		return 1;
	}

	//size_t n;
	int flag = 0;
	int flag_previos_part = 0;
	int has_line = 0;
	while (fgets(buf, BUF_SIZE, stdin)) {
		has_line = 1;
		flag = 0;
		int ptr = 0;
		while (buf[ptr]) {
			if (buf[ptr] == '\n') {
				flag_previos_part = 0;
			}
			if (!isspace(buf[ptr])) {
				flag = 1;
				flag_previos_part = 1;
			}
			ptr++;
		}
		if (flag == 0 && (ptr > 0 && buf[ptr - 1] == '\n')) {
			if (flag_previos_part == 0)fwrite("\n", sizeof(char), 1, pipe_to_child);
			flag_previos_part = 0;
		}
	}
	if (has_line && flag_previos_part == 0 && flag == 0) {
		fwrite("\n", sizeof(char), 1, pipe_to_child);
	}
	pclose(pipe_to_child);
	fclose(stdin);
	free(buf);


	return 0;
}
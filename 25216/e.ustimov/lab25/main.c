#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUF_SIZE 1024 * 4

int main(int argc, char** argv) {


	int fd[2];
	if (pipe(fd) != 0) {
		perror("ошибка открытия трубы через pipe");
		return 1;
	}
	pid_t pid;
	if ((pid = fork()) == -1) {
		perror("ошибка открытия трубы через pipe");
		close(fd[0]);
		close(fd[1]);
		return 1;
	}
	else if (pid == 0) { //child
		close(fd[0]);
		FILE* pipe_to_parent = fdopen(fd[1], "w");
		if (pipe_to_parent == NULL) {
			perror("ошибка обертки трубы через fdopen");
			return 1;
		}
		if (argc >= 2) {
			FILE* text = fopen(argv[1], "r");
			if (text == NULL) {
				perror("ошибка открытия файла через fopen");
			}
			else {
				dup2(fileno(text), STDIN_FILENO);
			}
		}
		char* buf = malloc(sizeof(char) * BUF_SIZE);
		if (buf == NULL) {
			perror("ошибка выделения памяти через malloc");
			return 1;
		}
		size_t n;
		while ((n = fread(buf, sizeof(char), BUF_SIZE, stdin)) > 0) {
			for (size_t i = 0; i < n; i++) {
				fputc(toupper(buf[i]), pipe_to_parent);
			}
		}
		fclose(pipe_to_parent);
		fclose(stdin);
		free(buf);
	}
	else {
		close(fd[1]);

		FILE* pipe_from_child = fdopen(fd[0], "r");
		if (pipe_from_child == NULL) {
			perror("ошибка обертки трубы через fdopen");
			return 1;
		}
		char* buf = malloc(sizeof(char) * BUF_SIZE);
		if (buf == NULL) {
			perror("ошибка выделения памяти через malloc");
			return 1;
		}
		size_t n;
		while ((n = fread(buf, sizeof(char), BUF_SIZE, pipe_from_child)) > 0) {
			fwrite(buf, sizeof(char), n, stdout);
		}

		fclose(pipe_from_child);
		free(buf);
		wait(NULL);
	}

	return 0;
}
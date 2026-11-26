GNU nano 4.7                                                               main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#define COUNT_RANDOM_NUMS 100
#define MAX_LEN_TEXT_OF_RAND_NUM COUNT_RANDOM_NUMS*2*2+1
#define BUF_SIZE 1024*4

int main() {
	FILE* pipe_to_child[2];

	if (p2open("sort -n", pipe_to_child) != 0) {
		perror("p2open");
		return 1;
	}
	srand(25216);

	for (int i = 0; i < COUNT_RANDOM_NUMS; i++) {
		fprintf(pipe_to_child[0], "%d\n", rand() % 100);
	}
	fflush(pipe_to_child[0]);
	close(fileno(pipe_to_child[0]));

	int num;
	int count = 0;
	while (fscanf(pipe_to_child[1], "%d", &num) == 1) {
		printf("%2d ", num);
		count++;
		if (count % 10 == 0)fputc('\n', stdout);
	}
	fflush(stdout);
	p2close(pipe_to_child);

	return 0;
}
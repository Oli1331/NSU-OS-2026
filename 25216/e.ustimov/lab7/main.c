#include <errno.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct Element_of_Vecotr_s
{
	size_t lenght;
	off_t shift;
} ElOfVector;

typedef struct Vector_s
{
	ElOfVector *line;
	int size;
	int count;
} Vector;

void init_vector(Vector *v)
{
	v->line = malloc(sizeof(ElOfVector));
	if (v->line == NULL)
		exit(1);
	v->size = 1;
	v->count = 0;
}

void push_vector(Vector *v, off_t s, size_t length)
{
	if (v->count >= v->size)
	{
		v->size *= 2;
		v->line = realloc(v->line, v->size * sizeof(ElOfVector));
		if (v->line == NULL)
			exit(1);
	}
	v->line[v->count].shift = s;
	v->line[v->count++].lenght = length;
}

size_t build_table(char *file_ptr, size_t size, Vector *table)
{
	int max_length = -1;
	int count_read_biyte;
	int global_position = 0;
	int length_line = 1;
	for (size_t i = 0; i < size; i++, length_line++)
	{
		if (file_ptr[i] == '\n')
		{
			push_vector(table, (off_t)(i - length_line + 1),
						length_line - 1);
			if (length_line > max_length)
				max_length = length_line;
			length_line = 0;
		}
	}

	if ((length_line - 1) > 0)
	{
		push_vector(table, size - length_line + 1, length_line);
		if (length_line > max_length)
			max_length = length_line;
	}

	return max_length;
}

void free_vector(Vector *v)
{
	free(v->line);
}

void print_table(Vector *v)
{
	printf("Table: \n");
	for (int i = 0; i < v->count; i++)
	{
		printf("line %d, shift %ld, length %zu\n", i + 1, v->line[i].shift,
			   v->line[i].lenght);
	}
}

void handler_sig(int n)
{
	return;
}

int main(int argc, char **argv)
{
	int fd;
	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("error open file\n");
		exit(1);
	}
	struct stat st;
	if (fstat(fd, &st) == -1)
	{
		perror("error get stat file\n");
		exit(1);
	}

	char *file_ptr = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file_ptr == NULL)
	{
		perror("error mmap file\n");
		exit(1);
	}
	int input = 1;
	int size_buf = 4096;
	char *buf = malloc(sizeof(char) * size_buf);
	if (buf == NULL)
	{
		perror("malloc is break\n");
		exit(1);
	}
	Vector table;
	init_vector(&table);
	if (signal(SIGALRM, handler_sig) == SIG_ERR)
	{
		perror("Error set handler");
		exit(1);
	}
	size_t max_length_line = build_table(file_ptr, st.st_size, &table);

	char *str = malloc(sizeof(char) * (max_length_line + 1));
	if (str == NULL)
	{
		perror("malloc is break");
		exit(1);
	}

	while (1)
	{
		alarm(3);
		if (read(STDIN_FILENO, buf, size_buf) < 1)
		{
			if (errno == EINTR)
			{
				printf("Content: \n");
				write(STDOUT_FILENO, file_ptr, st.st_size);
				free_vector(&table);
				free(buf);
				print_table(&table);
				break;
			}
			else
			{
				perror("Error read stream");
				break;
			}
		}
		errno = 0;
		input = (int)strtol(buf, NULL, 10);
		if (errno != 0 || input > table.count)
		{
			printf("bad number\n");
			continue;
		}
		else if (input == 0)
		{
			printf("END\n");
			break;
		}

		write(STDOUT_FILENO, file_ptr + table.line[input - 1].shift, table.line[input - 1].lenght);
		printf("\n");
		// lseek(fd, table.line[input - 1].shift, SEEK_SET);
		// read(fd, str, table.line[input - 1].lenght);
		// str[table.line[input - 1].lenght] = '\0';
		// printf("line №%d: %s\n", input, str);
	}
	free_vector(&table);
	free(buf);
	free(str);
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct Element_of_Vecotr_s {
    size_t lenght;
    off_t shift;
}ElOfVector;

typedef struct Vector_s {
    ElOfVector* line;
    int size;
    int count;
}Vector;

void init_vector(Vector* v) {
    v->line = malloc(sizeof(ElOfVector));
    if (v->line == NULL)exit(1);
    v->size = 1;
    v->count = 0;
}

void push_vector(Vector* v, off_t s, size_t length) {
    if (v->count >= v->size) {
        v->size *= 2;
        v->line = realloc(v->line, v->size * sizeof(ElOfVector));
        if (v->line == NULL)exit(1);

    }
    v->line[v->count].shift = s;
    v->line[v->count++].lenght = length;
}

size_t build_table(int fd, Vector* table, char* buf, int size_buf) {
    int max_length = -1;
    int count_read_biyte;
    int global_position = 0;
    int length_line = 1;
    while ((count_read_biyte = read(fd, buf, size_buf)) > 0) {
        for (int i = 0; i < count_read_biyte; i++, global_position++, length_line++) {
            if (buf[i] == '\n') {
                push_vector(table, (off_t)(global_position - length_line + 1), length_line - 1);
                if (length_line > max_length)max_length = length_line;
                length_line = 0;
            }
        }
    }
    if ((length_line-1) > 0) {
        push_vector(table, global_position - length_line + 1, length_line);
        if (length_line > max_length)max_length = length_line;
    }

    return max_length;
}

void free_vector(Vector* v) {
    free(v->line);
}

void print_table(Vector* v) {
    for (int i = 0; i < v->count; i++) {
        printf("line %d, shift %ld, length %zu\n", i+1, v->line[i].shift, v->line[i].lenght);
    }
}

int main(int argc, char** argv) {
    int fd = open(argv[1], O_RDONLY);
    int size_buf = 4096;
    char* buf = malloc(sizeof(char) * size_buf);
    if (buf == NULL)exit(1);
    Vector table;
    init_vector(&table);
    size_t max_length_line = build_table(fd, &table, buf, size_buf);
    char* str = malloc(sizeof(char) * (max_length_line + 1));
    if (str == NULL)exit(1);
    print_table(&table);
    int input = 1;
    while (1) {
        if (scanf("%d", &input) != 1 || input > table.count) {
            printf("bad number\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        if (input == 0)break;
        lseek(fd, table.line[input - 1].shift, SEEK_SET);
        read(fd, str, table.line[input - 1].lenght);
        str[table.line[input - 1].lenght] = '\0';
        printf("line №%d: %s\n", input, str);

    }
    free_vector(&table);
    free(buf);
    free(str);
    return 0;
}
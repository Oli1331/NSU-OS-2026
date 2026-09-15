#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 10000

typedef struct Node_of_list_s {
    char* str;
    struct Node_of_list_s* next_str;
}Node;

typedef struct List_s {
    Node* head;
    Node* last;
}List;

void list_init(List* ls) {
    Node* nd = malloc(sizeof(Node));
    if (nd == NULL)exit(1);
    ls->head = ls->last = nd;
    nd->next_str = NULL;
}

void add_str(List* ls, char* str, size_t len) {

    char* new_line = malloc((len + 1) * sizeof(char));
    if (new_line == NULL)exit(1);

    ls->last->str = new_line;
    strncpy(new_line, str, len);
    new_line[len] = '\0';

    Node* next = malloc(sizeof(Node));
    if (next == NULL)exit(1);
    next->next_str = NULL;

    ls->last->next_str = next;
    ls->last = next;
}

int read_str(List* ls, char* buf) {

    if (fgets(buf, MAX_LENGTH, stdin) == NULL)exit(1);
    size_t len = strlen(buf);
    if (len < 1 || buf[0] == '.')return 0;
    else {
        add_str(ls, buf, len-1);
        return 1;
    }
}

void print_list(List ls) {
    Node* ptr = ls.head;
    do {
        if (ptr->str != NULL)printf("%s\n", ptr->str);
        ptr = ptr->next_str;
    } while (ptr != NULL);
    
}

void free_list(List ls) {
    Node* ptr = ls.head;
    do {
        if (ptr->str != NULL)free(ptr->str);
        Node* cp = ptr;
        ptr = ptr->next_str;
        free(cp);
    } while (ptr != NULL);

}


int main() {
    List ls;
    list_init(&ls);
    char* buffer = malloc(MAX_LENGTH * sizeof(char));
    if (buffer == NULL)exit(1);

    while (read_str(&ls, buffer));
    print_list(ls);
    free_list(ls);
    free(buffer);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 4096

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

int my_getline(char** buf, size_t* len_buf) {
    if (*buf == NULL && *len_buf == 0) {
        *len_buf = 1;
        *buf = malloc(*len_buf * sizeof(char));
        if (*buf == NULL)exit(1);

    }
    int cnt_sym = 0;
    char symbol;
    while ((symbol = getc(stdin)) != '\n' && symbol != EOF) {
        if (*len_buf <= cnt_sym) {
            (*len_buf) *= 2;
            *buf = realloc(*buf, *len_buf * sizeof(char));
        }
        (*buf)[cnt_sym] = symbol;
        (cnt_sym)++;
    }
    (*buf)[cnt_sym]='\0';
    return cnt_sym;
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

    char* buffer=0;
    size_t len_buf = 0;

    while (my_getline(&buffer,&len_buf)){
        if(buffer[0]=='.')
            break;
        
        add_str(&ls,buffer,len_buf);
    }
    print_list(ls);
    free_list(ls);
    if(buffer!=NULL)free(buffer);
    

    return 0;
}

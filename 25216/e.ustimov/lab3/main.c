#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Реальный идентификатор пользователя RUID: %d\n", getuid());
    printf("Эффективный идентификатор пользователя EUID: %d\n", geteuid());

    FILE* file = fopen("file.txt", "r");
    if (file == NULL)perror("Open file is fail");
    else {
        printf("File is open!\n");
        fclose(file);
    }

    setuid(getuid());

    printf("Реальный идентификатор пользователя RUID: %d\n", getuid());
    printf("Эффективный идентификатор пользователя EUID: %d\n", geteuid());

    file = fopen("file.txt", "r");
    if (file == NULL)perror("Open file is fail");
     else {
        printf("File is open!\n");
        fclose(file);
    }
    
    return 0;
}
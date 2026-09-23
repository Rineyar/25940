#include <stdio.h>
#include <unistd.h>

//gcc -Wall -Wextra -Wpedantic -g my_file.c -o res
int main(void)
{
    printf("UID - %d\teUID - %d\n", getuid(), geteuid()); //Вывод id

    FILE *file = fopen("my_file.txt", "r");

    if(file == NULL)
    {
        printf("Файл не удалось открыть\n");
        perror("Файл не удалось открыть\n");
    } else {
        printf("Файл открыт: \n");

        int ch;

        while((ch = fgetc(file)) != EOF) //не просто же так задачка на файл
        {
            printf("%c", ch);
        }

        printf("\n=====\n");

        fclose(file);
    }

    setuid(getuid()); //UID = eUID

    file = fopen("my_file.txt", "r");

    printf("UID - %d\teUID - %d\n", getuid(), geteuid());

    if(file == NULL)
    {
        printf("Файл не удалось открыть\n");
        perror("Файл не удалось открыть\n");
    } else {
        printf("Файл открыт: \n");

        int ch;

        while((ch = fgetc(file)) != EOF)
        {
            printf("%c", ch);
        }

        printf("\n=====\n");

        fclose(file);
    }

    return 0;
}

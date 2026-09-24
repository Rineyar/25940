#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//gcc -Wall -Wextra -Wpedantic -g string_line.c -o res
int main(void)
{
    char line[1024];
    int i = 0;

    char **lines = malloc(sizeof(char*) * 1024);
    int cap = 4;
    int len = 0;

    while(1)
    {
        int ch = getchar();

        if(ch == '.')
        {
            break;
        }

        while(ch != EOF && ch != '\n')
        {
            line[i++] = ch;

            ch = getchar();
        }

        line[i++] = '\n';
        line[i++] = '\0';

        lines[len] = malloc(sizeof(char) * (i + 2));

        if(lines[len] == NULL)
        {
            perror("malloc");
            return 1;
        }

        memcpy(lines[len++], line, i * sizeof(char));

        i = 0;

        if(len == cap)
        {
            cap *= 2;

            lines = realloc(lines, cap);

            if(lines == NULL)
            {
                perror("realloc");
                return 1;
            }
        }
    }

    printf("%p\n", lines);

    for(int ii = 0; ii < len; ii++)
    {
        printf("%d - %p\n", ii, lines[ii]);
    }

    for(int ii = 0; ii < len; ii++)
    {
        printf("%s", lines[ii]);
    }

    return 0;
}

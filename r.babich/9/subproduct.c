#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(void)
{
    printf("Здесь могла быть ваша реклама:\n");

    pid_t pid = fork();

    if(pid == 0)
    {
        printf("И она здесь есть (наверное):\n");

        int e = execl("./", "cat longfile.txt", NULL);

        if(e == 0)
        {
            perror("execl");
            _exit(1);
        } else if(pid > 0)
        {
            waitpid(pid, NULL, 0);

            printf("РЕКлАМА ХЗ\n");
        } else {
            perror("fork");
        }
    }
    
    return 0;
}
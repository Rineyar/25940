#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(void)
{
    printf("Здесь могла быть ваша реклама:\n");

    pid_t pid = fork();

    if(pid == -1)
    {
        perror("fork");
    } else if(pid == 0)
    {
        printf("И она здесь есть (наверное):\n");

        int e = execlp("cat", "cat", "longfile.txt", NULL); //Исполняет бинарник cat в PATH в виде cat на longfile.txt

        if(e == -1)
        {
            perror("execlp");
            _exit(1);
        } 
    } else {
        waitpid(pid, NULL, 0);

        printf("РЕКлАМА ХЗ\n");
    }

    return 0;
}

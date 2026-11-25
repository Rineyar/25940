#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/select.h>

//#define THREAD_M
//#define SELECT_M
#define POLL_M

const size_t BUFSIZE = 64;
const size_t THREADS = 2;
const size_t CLIENTS = 16;

void clear(char *buf)
{
    for(size_t i = 0; i < BUFSIZE; i++)
    {
        buf[i] = (char)0;
    }
}

#ifdef THREAD_M
void *worker(void *arg)
{
    int fd = *((int *)arg);

    int run = 1;
    
    while(run == 1)
    {
        int client_fd = accept(fd, NULL, NULL); //lock

        char buf[BUFSIZE];

        while(1)
        {
            clear(buf);

            if(recv(client_fd, buf, BUFSIZE, 0) == 0)
            {
                break;
            }

            for(char *c = buf; *c != '\0'; c++)
            {
                *c = (char)toupper((int)*c);
            }

            printf("%s\n", buf);
        }

        close(client_fd);
    }

    return NULL;
}
#endif

int main(void)
{
    int fd = socket(AF_UNIX, SOCK_SEQPACKET, 0); //Открыли локальный сокет, получили дескриптор

    struct sockaddr_un addr = {0}; //Структура с адресом

    addr.sun_family = AF_UNIX; //Локальный сокет
    strncpy(addr.sun_path, "/tmp/server.sock", sizeof(addr.sun_path) - 1); //Общий файл

    unlink("/tmp/server.sock");

    bind(fd, (struct sockaddr *)&addr, sizeof(addr)); //Привязали

    listen(fd, CLIENTS);

    #ifdef SELECT_M

    fd_set server;
    FD_ZERO(&server);

    FD_SET(fd, &server);

    int max_fd = fd;

    char buf[BUFSIZE];

    while(1) //Гоняем подключения
    {
        fd_set clients = server;

        select(max_fd + 1, &clients, NULL, NULL, NULL); //Записать все fd

        if(FD_ISSET(fd, &clients)) //Если сервер есть
        {
            int client_fd = accept(fd, NULL, NULL); //Ловим голубя

            FD_SET(client_fd, &server); //Туда его

            if(client_fd > max_fd) //Сдвигаем
            {
                max_fd = client_fd;
            }
        }

        for(int client_fd = 0; client_fd <= max_fd; client_fd++) //Все клиенты
        {
            if(client_fd == fd) //Ладно, он не клиент
            {
                continue;
            }

            if(!FD_ISSET(client_fd, &clients)) //Это залётный
            {
                continue;
            }

            clear(buf);

            if(recv(client_fd, buf, BUFSIZE, 0) == 0) //Принимаем или скипаем
            {
                close(client_fd);

                FD_CLR(client_fd, &server);

                if(client_fd == max_fd)
                {
                    while(max_fd >= 0 && !FD_ISSET(max_fd, &server))
                    {
                        max_fd--;
                    }
                }

                continue;
            }

            for(char *c = buf; *c != '\0'; c++)
            {
                *c = (char)toupper((int)*c);
            }

            printf("%s\n", buf);
        }
    }

    #elifdef POLL_M

    struct pollfd fds[CLIENTS + 1];
    
    fds[0].fd = fd;
    fds[0].events = POLLIN;

    for(int i = 1; i <= CLIENTS; i++)
    {
        fds[i].fd = -1;
    }

    char buf[BUFSIZE];

    while (1)
    {
        poll(fds, CLIENTS + 1, -1); //Ловим

        if(fds[0].revents & POLLIN) //Сервер присутствует. Ещё и сравнение через бит
        {
            int client_fd = accept(fd, NULL, NULL);

            for(int i = 1; i <= CLIENTS; i++)
            {
                if(fds[i].fd == -1)
                {
                    fds[i].fd = client_fd;
                    fds[i].events = POLLIN;

                    break;
                }
            }
        }

        for(int i = 1; i <= CLIENTS; i++)
        {
            if(fds[i].fd == -1)
            {
                continue;
            }

            if(fds[i].revents & (POLLIN | POLLHUP | POLLERR)) //Чтение, закрытие, или вылет
            {
                int client_fd = fds[i].fd;

                clear(buf);

                if(recv(client_fd, buf, BUFSIZE, 0) == 0)
                {
                    close(client_fd);

                    fds[i].fd = -1;

                    continue;
                }

                for(char *c = buf; *c != '\0'; c++)
                {
                    *c = (char)toupper((int)*c);
                }

                printf("%s\n", buf);
            }
        }
    }

    #elifdef THREAD_M

    pthread_t threads[THREADS];

    for(size_t i = 0; i < THREADS; i++)
    {
        pthread_create(&threads[i], NULL, worker, &fd);
    }

    //Ждём потоки
    for(size_t i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    #endif

    return 0;
}

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <poll.h>
#include <time.h>
#include <stdlib.h>

const size_t BUFSIZE = 64;

void clear(char *buf)
{
    for(size_t i = 0; i < BUFSIZE; i++)
    {
        buf[i] = (char)0;
    }
}

int main(void)
{
    int fd = socket(AF_UNIX, SOCK_SEQPACKET, 0); //Открыли локальный сокет, получили дескриптор

    struct sockaddr_un addr = {0}; //Структура с адресом

    addr.sun_family = AF_UNIX; //Локальный сокет
    strncpy(addr.sun_path, "/tmp/server.sock", sizeof(addr.sun_path) - 1); //Общий файл

    connect(fd, (struct sockaddr *)&addr, sizeof(addr)); //Не привязали, а подключились

    char buf[BUFSIZE];

    strcpy(buf, "Sender2");

    struct timespec ts = { .tv_sec = 0, .tv_nsec = 500000000L };

    while(1)
    {
        // clear(buf);

        // scanf("%s", buf);

        // if(strncmp(buf, "exit", 4) == 0)
        // {
        //     break;
        // }

        // send(fd, buf, strlen(buf), 0);

        ts.tv_nsec += (long)((double)rand() / ((double)RAND_MAX + 1.0) * 200000001.0) - 100000000L;

        nanosleep(&ts, NULL);

        send(fd, buf, strlen(buf), 0);
    }

    return 0;
}
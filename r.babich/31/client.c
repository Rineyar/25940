#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <poll.h>

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

    while(1)
    {
        clear(buf);

        scanf("%s", buf);

        if(strncmp(buf, "exit", 4) == 0)
        {
            break;
        }

        send(fd, buf, strlen(buf), 0);
    }

    return 0;
}
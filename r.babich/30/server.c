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

    bind(fd, (struct sockaddr *)&addr, sizeof(addr)); //Привязали

    listen(fd, 1); //Мы только один слиент ждём

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

    return 0;
}

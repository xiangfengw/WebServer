#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>

#define SERV_PORT 6666
#define SERV_IP "127.0.0.1"

int main()
{
    int lfd, cfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_addr_len;
    char buf[BUFSIZ];
    int n;

    lfd = socket(AF_INET, SOCK_STREAM, 0);

    // memset(&serv_addr, 0, sizeof(serv_addr));
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);

    bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(lfd, 128);

    cli_addr_len = sizeof(cli_addr);
    cfd = accept(lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    while (1)
    {
        n = read(cfd, buf, sizeof(buf));
        for (int i = 0; i < n; i++)
        {
            buf[i] = toupper(buf[i]);
        }
        write(cfd, buf, n);
    }

    close(lfd);
    close(cfd);

    return 0;
}
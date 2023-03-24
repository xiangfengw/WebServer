#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <strings.h>

#include "wrap.h"

#define SERV_IP "0.0.0.0"
#define SERV_PORT 8888

void wait_child(int signal) // CHLD捕获函数
{
    while(waitpid(0, NULL, WNOHANG) > 0);
    return ;
}

int main(void)
{
    int lfd, cfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_addr_len;
    pid_t pid;
    char buf[BUFSIZ];
    int n;
    char temp[20] = "phillip, my son\n";

    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);
    Bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    Listen(lfd, 128);

    while(1) {
        cli_addr_len = sizeof(cli_addr);
        cfd = Accept(lfd, (struct sockaddr*)&cli_addr, &cli_addr_len);

        pid = fork();
        if(pid < 0) {
            perror("fork error");
            exit(1);
        } else if (pid == 0) {  // 子进程
            close(lfd);
            break;
        } else {  // 父进程 注册回收子进程
            close(cfd);
            signal(SIGCHLD, wait_child);
        }
    }

    if(pid == 0) {  // 子进程
        while(1) {
            n = Read(cfd, buf, sizeof(buf));
            if(n == 0) {  // client closed
                close(cfd);
                return 0;
            } else if(n == -1) { // 被中断的情况被wrap.c中封装的Read函数处理了
                perror("read error");
                exit(1);
            }
            else {
                write(cfd, buf, n);
                write(cfd, temp, 20);
            }
        }
    } else {  // 父进程 
        while(1) {

        }
    }

    return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#define PORT 8000
#define IP "192.168.126.140"
int chat(int newfd);
#define PRINT_ERR(msg) \
    do                 \
    {                  \
        perror("msg"); \
        return -1;     \
    } while (0)

int main(int argc, const char *argv[])
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0); // 创建 socket 套接字；
    if (sfd == -1)
        PRINT_ERR("socket");

    struct sockaddr_in usr_addr[1020] = {0};

    struct sockaddr_in sin; // 地址结构体
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = inet_addr(IP);
    if (0 > bind(sfd, (struct sockaddr *)&sin, sizeof(sin))) // 绑定自己 IP 和 端口
        PRINT_ERR("bind");

    if (listen(sfd, 100)) // 单次最多发送 100 个长度
        PRINT_ERR("listen");

    // 创立集合
    fd_set readfds, tempfds;
    FD_ZERO(&readfds);
    FD_ZERO(&tempfds);

    //添加文件描述符
    FD_SET(0, &readfds);
    FD_SET(sfd, &readfds);
    int maxfd = sfd;

    char buff[100] = "";
    int newfd;
    int s_res;
    int res;
    struct sockaddr_in cin;
    int addr_len = sizeof(cin);
    while (1)
    {
        tempfds = readfds;
        s_res = select(maxfd + 1, &tempfds, NULL, NULL, NULL);
        if (s_res < 0)
            PRINT_ERR("select");
        else if (s_res == 0)
        {
            printf("等待超时\n");
            continue;
        }
        printf("%d\n", __LINE__);
        for (int i = 0; i <= maxfd; i++)
        {
            if (!FD_ISSET(i, &tempfds))
            {
                continue;
            }
            if (0 == i)
            {
                printf("键盘准备就绪\n");
                fgets(buff, 10, stdin);
                printf("%s", buff);
            }
            else if (sfd == i)
            {
                newfd = accept(sfd, (struct sockaddr*)&cin, &addr_len); // 开启一块新的空间和其描述符
                if (-1 == newfd)
                    PRINT_ERR("accept");
                else
                    printf("链接已建立\n");
                maxfd = maxfd > newfd ? maxfd : newfd;
                FD_SET(newfd, &readfds);
                usr_addr[newfd - 4] = cin;
            }
            else
            {
                printf("收到消息:");
                bzero(buff, sizeof(buff));
                res = recv(i, buff, sizeof(buff), 0);
                if (res < 0)
                    PRINT_ERR("recv");
                if (res == 0)
                {
                    printf("%d已经退出\n", i);
                    FD_CLR(i, &readfds);
                    for (int j = maxfd; j > 2; j--)
                    {
                        if (!FD_ISSET(j, &readfds))
                            maxfd = j;
                    }
                }
                else
                {
                    printf("%s\n", buff); 
                }
            }
        }
    }

    close(sfd);
    return 0;
}

// int chat(int newfd)
// {
//     while (1)
//     {
//         char buff[100] = {0}; // 接受和打印消息的变量
//         int res;
//         while (res = recv(newfd, &buff, sizeof(buff), 0))
//         {
//             if (-1 == res)
//                 PRINT_ERR("recv");
//             // printf("res = %d\n",res);
//             printf("%s\n", buff);
//             send(newfd, buff, sizeof(buff), 0);
//         }
//     }
// }
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/select.h>
#include <sys/times.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PRINT_ERR(msg) \
    do                 \
    {                  \
        perror("msg"); \
        return -1;     \
    } while (0)

int main(int argc, char const *argv[])
{
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(0 > sfd)
        PRINT_ERR("socket");
    
    struct sockaddr_in cin;
    cin.sin_family = AF_INET;
    cin.sin_port = ntohs(8000);
    cin.sin_addr.s_addr = inet_addr("192.168.8.110");

    if(connect(sfd,(struct sockaddr*)&cin,sizeof(cin)) < 0)
        PRINT_ERR("connect");
    else printf("链接成功\n");

    // 建立select集合
    fd_set readfds,tempfds;
    FD_ZERO(&readfds);
    FD_ZERO(&tempfds);

    FD_SET(0,&readfds);
    FD_SET(sfd,&readfds);

    int max = sfd;
    char buff[100] = "";
    int res;
    while(1)
    {
        tempfds = readfds;
        select(max+1,&tempfds,NULL,NULL,NULL);
        if(FD_ISSET(0,&tempfds))
        {
            scanf("%s",buff);
            getchar();
            if(send(sfd,buff,sizeof(buff),0) < 0)
                PRINT_ERR("send");
        }
        if(FD_ISSET(sfd,&tempfds))
        {
            res = recv(sfd,buff,sizeof(buff),0);
            if(res < 0)
                PRINT_ERR("recv");
            else if(res == 0)
                printf("对方下线\n");
            else
                printf("%s\n",buff);
        }
    }
    close(sfd);
    return 0;
}
/*************************************************************************
	> File Name: client.c
	> Author: wang
	> Mail: dhkhl235@gmail.com
	> Created Time: 2018年03月29日 星期四 15时58分24秒
 ************************************************************************/
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXLINE 1500
#define SERVER_PORT 12345

typedef struct
{
    char name;
    struct sockaddr_in peeraddr;
}Msg;


int main()
{

    Msg msg;
    int confd;
    struct sockaddr_in serveraddr;
    char ipstr[INET_ADDRSTRLEN] = "111.230.171.212";

    confd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipstr, &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERVER_PORT);
    socklen_t len = sizeof(serveraddr);

    msg.peeraddr.sin_family = AF_INET;

    if ( (len = sendto(confd, "A", 1, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0)
    {
        printf("send error\n");
        exit(1);
    }

    recvfrom(confd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, &len);
    {
        //打印消息
        printf("name : %c\n", msg.name);
        char ip[16];
        printf("ip = %s   port = %d\n", inet_ntop(AF_INET, &msg.peeraddr.sin_addr.s_addr, ip, sizeof(ip)),  ntohs(msg.peeraddr.sin_port));
    }


    if ( (len = sendto(confd, "hello", 5, 0, (struct sockaddr *)&msg.peeraddr, sizeof(msg.peeraddr))) < 0)
    {
        perror("sendto cli");
    }
    while(1)
    {
        printf("数据将在5s后发送\n");
        if ( (len = sendto(confd, "send", 4, 0, (struct sockaddr *)&msg.peeraddr, sizeof(serveraddr))) < 0)
        {
            printf("send error\n");
            exit(1);
        }
        else
            printf("已经发送\n");
        char buff[1024];
        memset(buff, 0, sizeof(buff));
        recvfrom(confd, buff, sizeof(buff), 0, (struct sockaddr*)&msg.peeraddr, &len);
        printf("recv : %s\n", buff);
        sleep(2);
    }

    /*
    while(0)
    {

        char sendbuf[1024];
        char recvbuf[1024];

        fd_set rset;
        FD_ZERO(&rset);
        int nready;
        int maxfd;
        int fd_stdin = fileno(stdin);
        if(fd_stdin > sfd)
            maxfd = fd_stdin;
        else
            maxfd = sfd;

        while(1)
        {
            FD_SET(fd_stdin, &rset);
            FD_SET(sfd, &rset);
            nready = select(maxfd+1, &rset, NULL, NULL, NULL);

            if(nready == -1)
                perror("select");
            if(nready == 0)
                continue;

            if(FD_ISSET(sfd, &rset))
            {
                int len;
                if((len = recv(sfd, recvbuf, sizeof(recvbuf), 0))== -1)
                {
                    perror("recv");
                }
                else if(len == 0)
                {
                    printf("server close\n");
                    break;
                }
                else
                {
                    puts(recvbuf);
                }
                memset(recvbuf, 0, sizeof(recvbuf));
            }

            if(FD_ISSET(fd_stdin, &rset))
            {
                fgets(sendbuf, sizeof(sendbuf), stdin);
                if(send(sfd, sendbuf, strlen(sendbuf), 0) == -1)
                {
                    perror("send");
                }
                memset(sendbuf, 0, sizeof(sendbuf));
            }
        }

    }*/
    close(confd);
    return 0;
}

#include<netdb.h>
#include <poll.h>
#include<sys/sendfile.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include<sys/select.h>

#define ServIp "0.0.0.0"
#define ServPort 12345

int main()
{

    int client_count = 0;
    int sockfd;
    int on = 1;
    socklen_t client_len;

    char recvbuf[300] = {0};

    struct sockaddr_in serv_addr;
    struct sockaddr_in clientA;
    struct sockaddr_in clientB;
    struct sockaddr_in client_addr;

    bzero(&client_addr,sizeof(client_addr));
    bzero(&clientA,sizeof(serv_addr));
    bzero(&clientB,sizeof(serv_addr));
    bzero(&client_addr,sizeof(serv_addr));


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ServPort);
    inet_pton(AF_INET,ServIp,&serv_addr.sin_addr);


    if((sockfd = socket(AF_INET,SOCK_DGRAM,0))==-1)
    {
        perror("socket");
        exit(1);
    }
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))< 0)
    {
        perror("setsockopt");
    }
    if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    if(sockfd == -1)
    {
        printf("Socket() failed\n");
        exit(-1);
    }
    client_len = sizeof(struct sockaddr_in);
    while(1)
    {
        bzero(recvbuf,sizeof(recvbuf));
        int  n = recvfrom(sockfd,recvbuf,sizeof(recvbuf),0,(struct sockaddr *)&client_addr,&client_len);
        if(n == -1)
            perror("recvfrom");


        recvbuf[n] = 0;
        printf("client %s\n",recvbuf);
        printf("ip:%s\tport:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

        client_count++;


        if(recvbuf[0] == 'A')
        {
            clientA = client_addr;
        }
        if(recvbuf[0] == 'B')
        {
            clientB = client_addr;

        }

        bzero(recvbuf,sizeof(recvbuf));
        struct 
        {
            char name;
            struct sockaddr_in addr;
        }reply;
        if(client_count == 2)
        {
            printf("两个客户端都已连接\n\n");

            printf("现在给clientA发送clientB的IP Port\n");
            reply.name = 'B';
            reply.addr = clientB;
            int sd = sendto(sockfd,&reply,sizeof(reply),0,(struct sockaddr *)&clientA,client_len);
            if(sd == -1)
                perror("send");
            if(sd > 0)
                printf("message of clientB has sent to clientA\n\n\n");


            printf("现在给clientB发送clientA的IP Port\n");
            reply.name = 'A';
            reply.addr = clientA;
            sd = sendto(sockfd,&reply,sizeof(reply),0,(struct sockaddr *)&clientB,client_len);
            if(sd == -1)
                perror("send");
            if(sd > 0)
                printf("message of clientA has sent to clientB\n");

        }
    }
    return 0;
}

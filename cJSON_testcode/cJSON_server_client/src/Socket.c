
#include<netdb.h>
#include<wait.h>
#include<syslog.h>
#include <poll.h>
#include<signal.h>
#include <fcntl.h>
#include<sys/sendfile.h>
#include<pthread.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/epoll.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include<sys/select.h>
#define UDPSERV 1
#define UDPCLIENT 2
#define TCPSERV 3
#define TCPCLIENT 4
#define BACKLOG 10
#define ServIp "127.0.0.1"
#define ServPort 12345
struct sockaddr_in serv_addr;
int Socket(int type)   
{
    struct sockaddr_in client_addr;
    const int on = 1;
    int sockfd;
    bzero(&client_addr,sizeof(client_addr));
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ServPort);
    inet_pton(AF_INET,ServIp,&serv_addr.sin_addr);
    switch(type)
    {
        case UDPSERV:           /*udp服务器套接字*/
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
            break;    
        case UDPCLIENT:         /* udp客户端套接字 */
            if((sockfd = socket(AF_INET,SOCK_DGRAM,0))==-1)
            {
                perror("socket");
                exit(1);
            }
            break;
        case TCPSERV:           /* tcp服务器套接字 */
            if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(1);
            }
            if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))< 0)  /* 地址重复利用 */
            {
                perror("setsockopt");
            }
            if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1)
            {
                perror("bind");
                exit(1);
            }
            if(listen(sockfd,BACKLOG)==-1)
            {
                perror("listen");
                exit(1);
            }
            break;

        case TCPCLIENT:         /* tcp客户端套接字 */
            if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(1);
            }
            if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1 )
            {
                perror("connect");
                exit(-1);
            }

            break;
        default:
            sockfd = -1;
    }
        return sockfd;
}

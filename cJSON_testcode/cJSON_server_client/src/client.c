
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
#include "cJSON.h"
#define ServPort 12345
#define ServIp "127.0.0.1"

struct sockaddr_in serv_addr;

int main()
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ServPort);
    inet_pton(AF_INET,ServIp,&serv_addr.sin_addr);
    int n = connect(fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if(n < 0)
        perror("conn");
    cJSON *js = cJSON_CreateObject();
    cJSON_AddStringToObject(js,"type","mesg");
    cJSON_AddStringToObject(js,"dest","wangdong");
    cJSON_AddStringToObject(js,"src","gaotian");

    char buf[] = "hello,wangdong!";

    cJSON_AddNumberToObject(js,"length",strlen(buf));

    char *head = cJSON_Print(js);
    printf("\nhead length:%lu\n",strlen(head));

    struct iovec sendbuf[2];
    sendbuf[0].iov_base = (void *)head;
    sendbuf[0].iov_len = strlen(head);
    sendbuf[1].iov_base = (void *)buf;
    sendbuf[1].iov_len = strlen(buf);
    writev(fd,sendbuf,2);
}

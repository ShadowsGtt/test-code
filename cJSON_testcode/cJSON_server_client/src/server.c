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
#define ServIp "127.0.0.1"
#define ServPort 12345

struct sockaddr_in serv_addr;
struct sockaddr_in cli_addr;
extern int Socket(int);

int main()
{
    int listenfd = Socket(3);
    socklen_t sin_len;
    int new_fd = accept(listenfd,(struct sockaddr *)&cli_addr , &sin_len);
    printf("newfd:%d\n",new_fd);



    char buf[74] = {0};
    int n = recv(new_fd,buf,74,0);
    if(n < 0)
        perror("recv");

    cJSON *js = cJSON_Parse(buf); 
    cJSON *type = cJSON_GetObjectItem(js,"type");
    cJSON *dest = cJSON_GetObjectItem(js,"dest");
    cJSON *src  = cJSON_GetObjectItem(js,"src");
    cJSON *length = cJSON_GetObjectItem(js,"length");
    
    printf("\n消息头:\n");
    printf("type:%s\n",type->valuestring);
    printf("dest:%s\n",dest->valuestring);
    printf("src:%s\n",src->valuestring);
    printf("length:%d\n",length->valueint);


    char recvbuf[1024] = {0};
    recv(new_fd,recvbuf,length->valuedouble,0);
    printf("\n消息内容:\n");
    printf("%s\n",recvbuf);
    
}

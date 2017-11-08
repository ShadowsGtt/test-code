/*************************************************************************
	> File Name: server
	> Author: 
	> Mail: 
	> Created Time: 2017年05月01日 星期一 16时56分02秒
 ************************************************************************/

#include <mysql.h>
#include <errmsg.h>
#include <mysqld_error.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<sys/select.h>
#include<sys/epoll.h>
#include <fcntl.h>

#define BACKLOG 10
#define MYPORT 40009
#define MaxClients 500
#define localIP "127.0.0.1"
#define netIP "10.101.190.83"
#include"wechat.h"
int SlectResult(MYSQL *conn,char *sql,char *username,char *password)
{
    int ret = -1;
    MYSQL_RES *result; // mysql 记录集
    MYSQL_ROW row;
    int res = mysql_query(conn,sql);
    if (!res) {
        //printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(conn));
    } else {
        fprintf(stderr, "error %d: %s\n", mysql_errno(conn), mysql_error(conn));
    }
    result = mysql_store_result(conn);
    while ((row=mysql_fetch_row(result))) // 打印结果集
    {
        printf("%s,%s\n",row[0],row[1]);
        if((strcmp(username,row[0])==0) && (strcmp(password,row[1])==0))
            ret = 1;
        else
            ret = -1;
    }
    mysql_free_result(result); // 释放结果集
    return ret;
}

int main()
{
    char sql[100];
    int sin_size;// 
    int conn;   
    MYSQL *conn_mysql;  //数据库连接句柄
    conn_mysql = mysql_init(NULL);  //初始化链接句柄
 
    if (conn_mysql == NULL) { // 如果返回NULl说明初始化失败
        printf("mysql_init failed!\n"); 
        return EXIT_FAILURE;
    }
    if(Connect_mysql(conn_mysql,"localhost","root","gaotian1314","ChatSoft") != 0 )  //调用自己编写的连接函数 链接成功返回0
    {
        fprintf(stderr, "Connection failed!\n");  
    }
    else
    {
        printf("mysql Connect success\n");
    }
    int sockfd = Init();   //调用自己编写的Init 函数，返回被监听的文件描述符
    struct Infomation  message; //收发结构体
    init_scbuf(&message);   //初始化结构体
    int total = 0;    //链接成功的人数
    char recvbuf[2048];   // 用来存放recv函数收到的字符串
    char sendbuf[2048];  //用来存放要发送的字符串
    int epollfd = epoll_create(MaxClients);   //创建epoll文件描述符
    struct epoll_event rgfd; //用来注册新事件
    struct epoll_event Epoll_Cli[MaxClients]; //存放Epoll返回的可读写事件
    for(int i = 0;i <MaxClients;i++)
    {
        Epoll_Cli[i].data.fd = -1;
        clients[i].fd = -1;
    }

    rgfd.data.fd = sockfd;
    rgfd.events = EPOLLIN;
    if(epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&rgfd) == -1) //将监听的fd注册到epoll中
    {
        perror("register failed");
    }
    while(1)
    {
        int rtnum = epoll_wait(epollfd,Epoll_Cli,MaxClients,-1);
        for(int t = 0;t < rtnum ; t++)
        {

            if( (Epoll_Cli[t].data.fd == sockfd) && (Epoll_Cli[t].events & EPOLLIN) ) //有客户段连接服务器
            {
                sin_size = sizeof(their_addr);
                if(  (conn = (accept(sockfd,(struct sockaddr *)&their_addr,&sin_size)  )) == -1)
                {
                    perror("accept");
                    continue;
                }
                for(int k = 0; k < MaxClients;k++)
                {
                    if(clients[k].fd == -1)
                    {
                        Epoll_Cli[k].data.fd = conn;
                        clients[k].fd = conn;
                        break;
                    }
                }
                printf("新客户已经链接！IP:%s,Port:%d\n",inet_ntoa(their_addr.sin_addr),ntohs(their_addr.sin_port));
                printf("当前在线人数:%d\n",++total);
                rgfd.data.fd = conn;
                rgfd.events = EPOLLIN ;
                if(epoll_ctl(epollfd,EPOLL_CTL_ADD,conn,&rgfd) == -1) //将新连接的fd注册到epoll中
                {
                    perror("epoll_ctl");
                }
            }
            else if(Epoll_Cli[t].events & EPOLLIN) //有客户端发送消息
            {
                conn = Epoll_Cli[t].data.fd;
                if(conn == -1)
                    continue;
                int numbytes = recv(conn,recvbuf,sizeof(recvbuf),0);
                memcpy(&message,recvbuf,sizeof(recvbuf)); //将收到的字符串转换为结构体
                for(int i = 0;i < MaxClients;i++)
                {
                    if(clients[i].fd == conn)
                    {
                        strcpy(clients[i].name,message.NickName);
                        break;
                    }
                }
                if(numbytes == -1) //recv出错
                {
                    perror("recieve");
                    exit(0);
                }
                else if(numbytes == 0) //客户主动关闭链接
                {
                    printf("Port = %d已经关闭连接!\n",ntohs(their_addr.sin_port));
                    printf("当前在线人数:%d\n",--total);
                    clients[t].fd = -1;
                    Epoll_Cli[t].data.fd = -1;
                    close(conn);
                    memset(clients[t].name,0,sizeof(clients[t].name));
                }
                else
                {
                    printf("logo:%d\n",message.logo);
                    printf("username:%s\npassword:%s\n",message.username,message.password);
                    int logo = message.logo;
                    switch(logo)  //客户端请求
                    {
                        case 1:   //登录

                        sprintf(sql,"select username,password from clientmsg where username='%s' and password='%s'",message.username,message.password);
                            memcpy(sendbuf,&message,sizeof(message));
                            if(SlectResult(conn_mysql,sql,message.username,message.password) == 1)
                            {
                                message.flag = 1;
                                memcpy(sendbuf,&message,sizeof(message));
                                send(conn,sendbuf,sizeof(sendbuf),0);
                            }
                            else
                            {
                                message.flag = -1;
                                memcpy(sendbuf,&message,sizeof(message));
                                send(conn,sendbuf,sizeof(sendbuf),0);
                            }
                            break;
                        case 2:

                            break;
                        case 3:

                            break;
                        case 4:

                            break;
                        case 5:

                            break;
                        case 6:

                            break;
                        case 7:

                            break;
                        case 8:

                            break;
                        default:
                            break;
                    }
                }
            }
            memset(recvbuf,0,sizeof(recvbuf));
            memset(sendbuf,0,sizeof(sendbuf));
            bzero(sql,sizeof(sql));
            init_scbuf(&message);
        }
    }
    return 0;
}

#ifndef _WECHAT_H
#define _WECHAT_H

struct Client
{
    char IP[20];
    char PORT[20];
    char name[20];
    int fd;
};
struct sockaddr_in my_addr;
struct sockaddr_in their_addr;
struct Client clients[1024];
int exc_sql(MYSQL *conn,const char *sql) 
{
    int res = mysql_query(conn,sql );
    if (!res) {
        printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(conn));
    } else {
        fprintf(stderr, "error %d: %s\n", mysql_errno(conn), mysql_error(conn));
    }
}
void Error(MYSQL* conn) {
    printf("Connection error %d: %s\n", mysql_errno(conn), mysql_error(conn));
}
int Connect_mysql(MYSQL *conn,const char *IP,const char *username,const char *password,const char *database)
{
    if (mysql_real_connect(conn,IP, username, password, database, 0, NULL, 0)) {
        return 0;
    }
    else 
    {
        return -1;
    }
    
}
int Init()
{
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket");
        exit(1);
    }
    int on = 1;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = inet_addr(netIP);
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))< 0)//地址重复利用

        bzero(&(my_addr.sin_zero),8);
    if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if(listen(sockfd,BACKLOG)==-1)
    {
        perror("listen");
        exit(1);
    }  
    return sockfd;
}
void setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}
int Get_sendfd(char *msg,char *name,char *dname)
{
    int sendfd = -1;
    if(strcmp(dname,"\0")!= 0)
    {
        for(int k = 0 ; k < 50 ;k++)
        {
            if(strcmp(clients[k].name,dname) == 0)
            {
                sendfd = clients[k].fd;
                break;
            }
        }
    }
    return sendfd;
}

typedef struct Infomation
{
    int logo; //请求
    char username[16]; //用户名
    char password[16]; //密码
    int flag;     //服务器回复客户端请求是否成功  0.成功 1.失败
    int fd;     //文件描述符
    char NickName[16]; //昵称
    char message[1024]; //聊天消息
    double money; //红包金额
    unsigned int count;  //抢红包人数
}SCbuf;

void init_scbuf(SCbuf *message) // 初始化结构体函数,每次send或recv完结构体后都要清空
{
    message->logo = -1;
    bzero(message->username,sizeof(message->username));
    bzero(message->password,sizeof(message->password));
    message->flag = -1;
    bzero(message->NickName,sizeof(message));
    message->money = 0;
    message->count = 0;
}
#endif

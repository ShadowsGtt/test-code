 #include<pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<netdb.h>
#include <arpa/inet.h>
#include<sys/sendfile.h>
#include <unistd.h>
#include <poll.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#define MaxClients 1024 
#define SerIP "111.230.171.212"
#define PORT 40009
#include"wechatcli.h"
void AllMenu()
{
    putchar(10);
    printf("****************************************\n");
    printf("*            1.好友管理                *\n");
    printf("*            2.群管理                  *\n");
    printf("*            3.聊天                    *\n");
    printf("*            4.红包                    *\n");
    printf("*            5.退出                    *\n");
    printf("****************************************\n");
    printf("请选择:");

}
void FriendManageMenu() //好友管理菜单
{
    putchar(10);
    printf("****************************************\n");
    printf("*            1.添加好友                *\n");
    printf("*            2.删除好友                *\n");
    printf("*            3.查看我的好友            *\n");
    printf("*            4.修改好友备注            *\n");
    printf("*            5.返回上一                *\n");
    printf("*            6.退出                    *\n");
    printf("****************************************\n");
    printf("请选择:");
    
}
void GroupManageMenu() //群管理菜单
{
    putchar(10);
    printf("****************************************\n");
    printf("*            1.加入群                  *\n");
    printf("*            2.退群                    *\n");
    printf("*            3.查看已加入群            *\n");
    printf("*            4.返回上一层              *\n");
    printf("*            5.退出                    *\n");
    printf("****************************************\n");
    printf("请选择:");
    
}
void ChatMenu() //聊天菜单
{
    putchar(10);
    printf("****************************************\n");
    printf("*            1.群聊天                  *\n");
    printf("*            2.一对一聊天              *\n");
    printf("*            3.返回上一层              *\n");
    printf("*            4.退出                    *\n");
    printf("****************************************\n");
    printf("请选择:");
    
}
void RedPacketMenu() //红包菜单
{
    
}
void LoginMenu()
{
    putchar(10);
    printf("*************************************\n");
    printf("*            1.登录                 *\n");
    printf("*            2.注册                 *\n");
    printf("*            3.退出                 *\n");
    printf("*************************************\n");
    printf("请选择:");

}
void SendFile(struct Infomation *message)   //sendfile零拷贝传输文件
{
    message->logo = 4;//标记为发送文件
    int fd = open("source",O_RDONLY);
    //FILE *fp = fdopen(fd,"r");
    int ret = sendfile(message->fd,fd,NULL,50);
    if(ret == -1)
    {
        printf("sendfile error\n");
        exit(0);
    }

}
void SignIn(struct Infomation *message)   //登录
{
    message->logo=1;//标记为登录
    printf("\n请输出您的用户名:");
    scanf("%s",message->username);
    printf("请输入密码:");
    scanf("%s",message->password);
    char buf[2048];
    memcpy(buf,message,sizeof(*message)); 
    send(message->fd,buf,sizeof(buf),0);
    init_scbuf(message);
    bzero(buf,sizeof(buf));
    int numbytes=recv(message->fd,buf,sizeof(buf),0);
    memcpy(message,buf,sizeof(buf)); 
    if(numbytes == -1)
    {
        perror("recv");
        exit(1);
    }
    else if(numbytes == 0)
    {
        printf("Server has closed\n");
        exit(1);
    }
    else
    {
        //printf("flag:%d\n",message->flag);
        if(message->flag == 1)
            printf("\n***********************登录成功!***********************\n");
        else
        {
            printf("*****************用户名或密码错误!*******************");
            SignIn(message);
        }
        init_scbuf(message);
    }

}
void SignUp(struct Infomation *message) //注册
{
    message->logo=2;//标记为注册
    printf("\n请输出您的用户名:");
    scanf("%s",(*message).username);
    printf("请输入密码:");
    scanf("%s",(*message).password);
    printf("请输入名字:");
    fgets(message->NickName,sizeof(message->NickName),stdin);
    char buf[2048];
    memcpy(buf,message,sizeof(*message)); 
    send(message->fd,buf,sizeof(buf),0);
    init_scbuf(message);
    bzero(buf,sizeof(buf));
    int numbytes=recv(message->fd,buf,sizeof(buf),0);
    memcpy(message,buf,sizeof(buf)); 
    if(numbytes == -1)
    {
        perror("recv");
        exit(1);
    }
    else if(numbytes == 0)
    {
        printf("与服务器断开链接...\n");
        exit(1);
    }
    /*else
    {
        if(message->flag == 1)
        {

        }
        else
        {
            
        }
        init_scbuf(message);
    }*/

}
void Login(struct Infomation *message)
{
    int select;
    char ch;
    LoginMenu();
    scanf("%d",&select);
    while((ch = getchar()) != '\n' && ch != EOF);
    switch(select)
    {
        case 1:
            SignIn(message);
            break;
        case 2:
            SignUp(message);
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("\n******************选择错误,请重新选择**************\n");
            Login(message);
            break;
    }
}
void Main(struct Infomation *message)
{
    int choose;
    char ch;
    do
    {
        AllMenu();  //总菜单
        scanf("%d",&choose);
        while((ch = getchar()) != '\n' && ch != EOF); //清空输入缓存区的回车
        switch(choose)
        {
            case 1:

                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:

                break;
            default:

                printf("\n**********错误的选择!**************\n");
                Main(message);
                break;
        }

    }while(1);
    
}
int main()
{
    int sockfd,numbytes;
    struct hostent *he;
    struct sockaddr_in their_addr;


    struct Infomation sendmessage;   //创建发信息结构体
    struct Infomation recvmessage;   //创建收信息结构体
    init_scbuf(&recvmessage);
    init_scbuf(&sendmessage);
    if((he = gethostbyname(SerIP))==NULL)
    {
        herror("gethostbyname");
        exit(1);
    }
    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero),8);
    if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)) == -1)
    {
        printf("connect failed\n");
        exit(-1);
    }
    else
        printf("connect server success!\n");

    sendmessage.fd = sockfd;
    //登录
    Login(&sendmessage);
    //Main(&sendmessage);
    
    return 0;
}

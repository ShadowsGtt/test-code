/*************************************************************************
	> File Name: wechatcli.h
	> Author: 
	> Mail: 
	> Created Time: 2017年10月28日 星期六 10时50分08秒
 ************************************************************************/

#ifndef _WECHATCLI_H
#define _WECHATCLI_H

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

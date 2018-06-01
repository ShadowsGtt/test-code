/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年05月03日 星期四 21时58分53秒
 ************************************************************************/

#include<iostream>
#include"test.pb.h"
#include<string.h>
using namespace std;

int main()
{
    WeChatPro::Login_Head test;
    test.set_data_size(40);
    test.set_username("gt1025814447");
    test.set_password("gaotian000");

    string result;
    test.SerializeToString(&result);
    cout<<"size:"<<result.size()<<endl; 
    cout<<"ater encoding::"<<result<<endl;

    cout<<"显示:"<<endl;
    test.ParseFromString(result);
    cout<<"data_size:"<<test.data_size()<<endl;
    cout<<"username :"<<test.username()<<endl;
    cout<<"password :"<<test.password()<<endl;
    
}


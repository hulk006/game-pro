//
// Created by yh on 17-8-7.
//

#ifndef POKER_AI_CONFIG_H
#define POKER_AI_CONFIG_H

#include <iostream>
#include "card.h"
#include <sstream>
#include "tools.h"
//××××××××××××××××××××××××××××××××××××××××××××××××××××××××
//这是服务器的监听端口，所有客户端的buf必须通过此端口发送才能被监听到

class ConfigTCP
{
public:
    ConfigTCP();
    ~ConfigTCP();
    #define PORT_  2245
    #define MAXSIZE_ 1024
    int  serv_sockfd_;//服务器的socket，调用socket函数返回的文件描述符//创建套接字（socket）：在网络中用来描述计算机中不同程序与其他计算机程序的通信方式。
    int client_sockfd_;
    int connect_flag_;//长短链接的标志，0短连接，1长连接
    char sendbuf_[MAXSIZE_];//处理结果的字符串
    char buf_[MAXSIZE_];//接受牌局信息的字符串

    Ai_input ai_input_;

    std::vector<std::string> buf_split_;
    struct sockaddr_in server_addr_;//服务
    struct sockaddr_in client_addr_;//客户

    void connection();
    bool acceptRecv();//接受客户端的请求
    bool recvMsg();//接受客户端的消息，并且判断格式是否正确
    void errorSend(); //格式错无发送的是error
    bool msgToCards();//把接受到的手牌和公的消息转化为Cards
    bool msgToAiInput();

};


#endif //POKER_AI_CONFIG_H

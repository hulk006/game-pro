//
// Created by yh on 17-8-7.
//

#ifndef POKER_AI_CONFIG_H
#define POKER_AI_CONFIG_H

#include <iostream>
#include "card.h"
#include "mygame.h"
#include <sstream>
#include "daemon.h"
//××××××××××××××××××××××××××××××××××××××××××××××××××××××××
//这是服务器的监听端口，所有客户端的buf必须通过此端口发送才能被监听到
//#define PORT  2245
//#define MAXSIZE 1024

class Config
{
    int  serv_sockfd_;
    int client_sockfd_;
    struct sockaddr_in server_addr_;//服务
    struct sockaddr_in client_addr_;//客户

};







#endif //POKER_AI_CONFIG_H

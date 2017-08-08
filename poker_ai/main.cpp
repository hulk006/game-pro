//
// Created by yanghao  on 17-6-21.
//this cpp is a sever for the poker game
//when recieve a buf we can deal it
//then give a decision for ai to fold call raise check all_in



#include <iostream>
#include "card.h"
#include "mygame.h"
#include <sstream>
#include "config.h"
#include "daemon.h"
//××××××××××××××××××××××××××××××××××××××××××××××××××××××××
//这是服务器的监听端口，所有客户端的buf必须通过此端口发送才能被监听到
#define PORT  2245
#define MAXSIZE 1024

int main()
{
    ConfigTCP configTCP;
    configTCP.connection();//建立链接绑定端口，并且允许地址复用
    while (1)
    {
        if (configTCP.acceptRecv() == true)//接受成工才能进行
        {
            while (1)
            {
                if (configTCP.recvMsg() == true)//通过格式检测，才能进行否则报错
                {

                    Mygame start;//进行模拟的游戏的类
                    Ai_input ai_input;
                    std::string hole = configTCP.buf_split_[2];
                    start.setAiInput(ai_input, configTCP.buf_split_,configTCP);
                    start.ai_fcr(ai_input);
                    int fcr = start.ai_out_.fcr_bet_;
                    std::cout << "理论上下注" << fcr << std::endl;
                    std::stringstream ss;
                    ss << "poker_fcr;" << configTCP.buf_split_[1] << ";" << start.ai_out_.fcrca_ << ";" << "send_end;";
                    strcpy(configTCP.sendbuf_, ss.str().c_str());
                    send(configTCP.client_sockfd_, configTCP.sendbuf_, strlen(configTCP.sendbuf_), 0);
                    std::cout << "sendbuf2=" << configTCP.sendbuf_ << std::endl;
                    memset(configTCP.sendbuf_, 0, sizeof(configTCP.sendbuf_));
                    memset(configTCP.buf_, 0, sizeof(configTCP.buf_));//清零
                    if (configTCP.connect_flag_ == 0) //短连接
                    {
                        close(configTCP.client_sockfd_);
                    }
                    break;
                }
                else
                {
                    configTCP.errorSend();
                    break;
                }
            }
        }
        else
        {
            configTCP.errorSend();
            continue;
        }
    }
       puts("exit success");
       exit(EXIT_SUCCESS);
       return 0;
}



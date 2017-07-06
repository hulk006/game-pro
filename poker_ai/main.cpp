//
// Created by yanghao  on 17-6-21.
//this cpp is a sever for the poker game
//when recieve a buf we can deal it
//then give a decision for ai to fold call raise check all_in



#include <iostream>
#include "card.h"
#include "mygame.h"
#include <sstream>
#include "daemon.h"
//××××××××××××××××××××××××××××××××××××××××××××××××××××××××
//这是服务器的监听端口，所有客户端的buf必须通过此端口发送才能被监听到
#define PORT  2245
#define MAXSIZE 1024

int main()
{
    //守护进程函数，用于保持程序一直运行，但是服务器上只能运行一次这个程序，否则会造成电脑卡顿
    //daemon(0,0);
    init_daemon();
    int serv_sockfd;//服务器的socket，调用socket函数返回的文件描述符
    //创建套接字（socket）：在网络中用来描述计算机中不同程序与其他计算机程序的通信方式。
    if ((serv_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "create socket failed\n");
        exit(EXIT_FAILURE);
    }
    puts("create socket success");
    //定义服务端套接口数据结构
    struct sockaddr_in server_addr;//服务
    struct sockaddr_in client_addr;//客户
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //命名socket, 我们知道， 绑定"协议， ip地址,  端口号"这三个要素， 而命名就是通过调用bind函数把socket与这三个要素绑定一起来。
    if (bind(serv_sockfd, (struct sockaddr *) (&server_addr), sizeof(struct sockaddr)) < 0) {
        fprintf(stderr, "bind failed \n");
        exit(EXIT_FAILURE);
    }
    puts("bind success\n");
    //如果听成功
    if (listen(serv_sockfd, 10) < 0) {
        fprintf(stderr, "listen failed \n");
        perror("listen fail\n");;
        exit(EXIT_FAILURE);
    }
    puts("listen success,waiting a buf info!");

    //服务器一直初一监听状态，不会关闭。
    while (1) {
           int  client_sockfd;
           socklen_t sin_size = sizeof(struct sockaddr_in);
           client_sockfd = accept(serv_sockfd, (struct sockaddr *) (&client_addr), &sin_size);
           if ( client_sockfd < 0)
           {
               fprintf(stderr, "accept failed \n");
               close(client_sockfd);
               continue;

           }
           printf("accepted a new connetction\n");
           printf("new socket id is %d\n", client_sockfd);
           printf("Accept clent ip is %s\n", inet_ntoa(client_addr.sin_addr));
           printf("Connect successful please input message\n");
            while(1)
            {
                char sendbuf[MAXSIZE];//处理结果的字符串
                char buf[MAXSIZE];//接受牌局信息的字符串
		memset(sendbuf, 0, sizeof(sendbuf));
                memset(buf, 0, sizeof(buf));//清零
                int recieve = recv(client_sockfd, buf, sizeof(buf), 0);
                if(strcmp(buf,"")==0)
                {
                    close(client_sockfd);
                    break;
                }
                char *first = "p";
                //如果接收到信息，才继续执行
                if(buf[0] == *first)
                {
                    Mygame start;//进行模拟的游戏的类
                    std::string buf_string = buf;
                    std::vector<std::string> buf_split;
                    buf_split.clear();
                    char *split = ";";
                    start.splitString(buf_string, ";", buf_split);//安照固定格式对字符串进行分段
                    std::vector<std::string> connect_str;
                    start.splitString(buf_split[8], ",", connect_str);
                    int connect_flag = std::stoi(connect_str[1]);//long or short connecting;
                    //如果标志为正确才能够执行ai的决策，否则重新开始
                    if (buf_split[0] == "poker_info") {
                        //通信的交互位
                        std::cout << "收到一个请求，游戏id为" << buf_split[1] << std::endl;
                        Ai_input ai_input;
                        start.setAiInput(ai_input, buf_split);
                        start.ai_fcr(ai_input);
                        int fcr = start.ai_out_.fcr_bet_;
                        std::cout << "理论上下注" << fcr << std::endl;
                        std::stringstream ss;
                        ss << "poker_fcr;" << buf_split[1] << ";" << start.ai_out_.fcrca_ << ";" << "send_end;";
                        strcpy(sendbuf, ss.str().c_str());
                        send(client_sockfd, sendbuf, strlen(sendbuf), 0);
                        std::cout << "sendbuf2=" << sendbuf << std::endl;
                        memset(sendbuf, 0, sizeof(sendbuf));
                        memset(buf, 0, sizeof(buf));//清零
                        if(connect_flag == 0)
                        {
                            //sleep(2);
                            close(client_sockfd);
                        }
                        continue;
                    }
		 else {
                        strcpy(sendbuf, "error");
                        send(client_sockfd, sendbuf, strlen(sendbuf), 0);
                        std::cout << "waiting a buf1" << std::endl;

                        close(client_sockfd);
                        //sleep(1);
                        continue;
                    }
                }
                else
                {
                    strcpy(sendbuf, "error");
                    send(client_sockfd, sendbuf, strlen(sendbuf), 0);
                    std::cout << "waiting a buf2" << std::endl;
                    close(client_sockfd);
                    continue;
                }

            }

       }
       puts("exit success");
       exit(EXIT_SUCCESS);
       return 0;
}



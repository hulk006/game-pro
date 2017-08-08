//
// Created by yh on 17-8-7.
//
#include "config.h"

ConfigTCP::ConfigTCP()
{
    //定义服务端套接口数据结构
    bzero(&server_addr_, sizeof(struct sockaddr_in));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(PORT_);
    server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    ai_input_.hole_cards_.cards_.clear();
    ai_input_.com_cards_.cards_.clear();
    ai_input_.game_pool_ = 0;
    ai_input_.current_bet_ = 0;
    ai_input_.chip = 0;
    ai_input_.check_ = 0;
    //将发送和接受的字符串清空
    memset(sendbuf_, 0, sizeof(sendbuf_));
    memset(buf_, 0, sizeof(buf_));//清零
}

ConfigTCP::~ConfigTCP()
{
    //将发送和接受的字符串清空
    memset(sendbuf_, 0, sizeof(sendbuf_));
    memset(buf_, 0, sizeof(buf_));//清零
}


void ConfigTCP::connection()
{

    if ((serv_sockfd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "create socket failed\n");
        exit(EXIT_FAILURE);
    }
    puts("create socket success");
    int  optval=1;
    setsockopt(serv_sockfd_,SOL_SOCKET,SO_REUSEADDR,(char*)&optval,sizeof(optval));

    //命名socket, 我们知道， 绑定"协议， ip地址,  端口号"这三个要素， 而命名就是通过调用bind函数把socket与这三个要素绑定一起来。
    if (bind(serv_sockfd_, (struct sockaddr *) (&server_addr_), sizeof(struct sockaddr)) < 0) {
        fprintf(stderr, "bind failed \n");
        exit(EXIT_FAILURE);
    }
    puts("bind success\n");
    //如果听成功
    if (listen(serv_sockfd_, 10) < 0) {
        fprintf(stderr, "listen failed \n");
        perror("listen fail\n");;
        exit(EXIT_FAILURE);
    }
    puts("listen success,waiting a buf info!");
    //服务器一直初一监听状态，不会关闭。
}
bool ConfigTCP::acceptRecv()
{
    socklen_t sin_size = sizeof(struct sockaddr_in);
    client_sockfd_ = accept(serv_sockfd_, (struct sockaddr *) (&client_addr_), &sin_size);
    if (client_sockfd_ < 0)
    {
        fprintf(stderr, "accept failed \n");
        close( client_sockfd_);
        return 0;
    }
    printf("accepted a new connetction\n");
    printf("new socket id is %d\n",  client_sockfd_);
    printf("Accept clent ip is %s\n", inet_ntoa(client_addr_.sin_addr));
    printf("Connect successful please input message\n");
    return 1;
}

bool ConfigTCP::recvMsg()
{
    memset(sendbuf_, 0, sizeof(sendbuf_));
    memset(buf_, 0, sizeof(buf_));//接受和发送之前清零缓存的东西
    int recieve = recv(client_sockfd_, buf_, sizeof(buf_), 0);

    if(strcmp(buf_,"")==0)//如果为空，返回0
    {
        close(client_sockfd_);
        return false;
    }

    std::string buf_string = buf_;
    buf_split_.clear();
    char *split = ";";
    splitString(buf_string, ";", buf_split_);//安照固定格式对字符串进行分段
    //检测请求消息的格式
    if (buf_split_.size()== 10
        && buf_split_[0].find("poker_info") != std::string::npos
        && buf_split_[1] != ""
        && buf_split_[2].find("hole,") != std::string::npos
        && buf_split_[3].find("com_card") != std::string::npos
        && buf_split_[4].find("current_bet,") != std::string::npos
        && buf_split_[5].find("game_pool_,") != std::string::npos
        && buf_split_[6].find("chip,") != std::string::npos
        && buf_split_[7].find("check_,") != std::string::npos
        && buf_split_[8].find("connect,") != std::string::npos)
    {
        std::vector<std::string> hole,com,string_vec,connect;
        splitString(buf_split_[2], ",", hole);
        splitString(buf_split_[3], ",", com);//检测手牌0～5
        if(hole.size() != 2+1)//检测手牌==2
        {

            return false;
        }
        if (com.size() > 6)
        {

            return false;
        }
        for(int i = 4;i <= 7;++i)//检测其他的变量都至少有两个值
        {
            splitString(buf_split_[i], ",", string_vec);
            if(string_vec.size() != 2 )
                return false;
        }

        splitString(buf_split_[8], ",", connect);
        if(string_vec.size() != 2 )
        {
            return false;
        }
        else
        {
            connect_flag_ = std::stoi(connect[1]);//long or short connecting;
        }

        //return msgToCards()&& msgToAiInput();//返回是否是一个合理的牌
        return msgToCards();
    }
    else
    {
        return false;
    }
}

void ConfigTCP::errorSend()
{
    strcpy(sendbuf_, "error");//把error复制给sendbuf char型数组
    std::cout<<"fatal error: receive msg is not legal!"<<std::endl;
    send(client_sockfd_, sendbuf_, strlen(sendbuf_), 0);//发送的字符串
    close(client_sockfd_);
}

bool ConfigTCP::msgToCards()
{
    return stringToCards(buf_split_[2], ai_input_.hole_cards_) && stringToCards(buf_split_[3], ai_input_.com_cards_);
}

bool ConfigTCP::msgToAiInput()//因为对格式要求太严格了，不太好用的。
{
    return stringToAiInput(buf_split_ ,ai_input_);
}




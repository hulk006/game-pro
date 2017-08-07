#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<errno.h>
#include <iostream>


#define PORT 2245
int main(int argc ,char **argv )
{

    int sockfd;//客户端的socket
    char buffer[2014];

    struct sockaddr_in server_addr;
    struct hostent *host;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)//如果没有建立socket，就错误退出
    {
        fprintf(stderr, "Socket Error is %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    //设置server_addr的值，服务器的信息
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //定义发送字符，接受字符
    char sendbuf[1024]= "";
    char recvbuf[2014];
    memset(recvbuf, 0, sizeof(recvbuf));

    //客户端发出请求，尝试与服务器建立链接，如果失败就报错
    if (connect(sockfd, (struct sockaddr *) (&server_addr), sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "Connect failed\n");
        exit(EXIT_FAILURE);
    }
    char *ai_input;
    if(argc == 1)
    {
        ai_input =  "poker_info;11;hole,2H,6C;com_card,;current_bet,100; game_pool_,180;chip,4000;check_,0;connect,0;send_end;";
    }
    else if(argc == 2)
    {
        ai_input = argv[1];
    }
    else
    {
        fprintf(stderr,"error:input param id mismatched!\n");//错误输出
        exit(EXIT_FAILURE);
    }
    //赋值字符串
    strcpy(sendbuf, ai_input);
    send(sockfd, sendbuf, strlen(sendbuf), 0);
    std::cout<<"sendbuf="<<sendbuf<<std::endl;
    int recv_flag = recv(sockfd, recvbuf, sizeof(recvbuf), 0);//接受服务器发送来的信息存入recvbuf
    if(recv_flag >=0 )
    {
        std::cout<<"recvbuf="<<recvbuf<<std::endl;
    }
    else
    {
        fprintf(stderr,"the server is not working");
    }

    //close(sockfd);
    return 0;

}

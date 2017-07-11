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
int main() {


    while (1) {

        int sockfd;
        char buffer[2014];

        struct sockaddr_in server_addr;
        struct hostent *host;
        int nbytes;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            fprintf(stderr, "Socket Error is %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        char sendbuf[1024]= "";
        char recvbuf[2014];

        //客户端发出请求
        if (connect(sockfd, (struct sockaddr *) (&server_addr), sizeof(struct sockaddr)) == -1) {
            fprintf(stderr, "Connect failed\n");
            exit(EXIT_FAILURE);
        }

        char ai_input[] = "poker_info;111111;hole,2H,6C;com_card,;current_bet,1000; game_pool_,150;chip,4000;check_,0;connect,0;send_end;";
        //char ai_input[] = "";
        //fgets(ai_input, sizeof(ai_input), stdin);
        strcpy( sendbuf, ai_input);
        send(sockfd, sendbuf, strlen(sendbuf), 0);
        std::cout<<"sendbuf="<<sendbuf<<std::endl;

        if (strcmp(ai_input, "exit\n") == 0)
            break;
        recvbuf;
        std::cout<<"recvbuf1="<<recvbuf<<std::endl;
        int recv_flag = recv(sockfd, recvbuf, sizeof(recvbuf), 0);
        if(recv_flag >=0 )
        {
            std::cout<<"recvbuf="<<recvbuf<<std::endl;
            memset(sendbuf, 0, sizeof(sendbuf));
            memset(recvbuf, 0, sizeof(recvbuf));
            sleep(1);

        } else
        {
            continue;
        }
    }
    //close(sockfd);
    exit(EXIT_SUCCESS);
    return 0;

}

//
// Created by yh on 17-8-7.
//

#ifndef POKER_AI_TOOLS_H
#define POKER_AI_TOOLS_H


#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include "card.h"
#include <map>

struct Ai_input
{
    Cards hole_cards_;
    Cards com_cards_;
    int current_bet_;
    int game_pool_;
    int chip;
    bool check_;
};


void splitString(const std::string &input,char *split,std::vector<std::string> &output);

bool stringToCard(const std::string &card_string, Card &card);//能够把一张牌牌的字符串转化成为card

bool stringToCards(const std::string &cards_string, Cards &cards);//把多张牌转化为

bool stringToAiInput(const std::vector<std::string> &buf_split_, Ai_input &ai_input);

#endif //POKER_AI_TOOLS_H

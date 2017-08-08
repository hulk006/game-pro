//
// Created by yh on 17-6-23.
//

#ifndef POKER_AI_MYGAME_H
#define POKER_AI_MYGAME_H

#include "ai.h"
#include "player.h"
#include "machine.h"
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include "config.h"
#include "tools.h"

const std::string CARDTYPE[11] = {"", "高牌", "一对", "两对",
                                  "三条", "顺子", "同花", "葫芦", "四条", "同花顺", "皇家同花顺"};
const std::string  DECISION[5] = {"fold", "call","raise", "Check","all_in"};


struct Ai_out
{
    int fcrca_;//reprents "fold", "call","raise", "Check","all_in"
    float strength_hand_;
    float RR_;
    int fcr_bet_;
};


class Mygame
{
public:
    Mygame();
    ~Mygame();
    Ai ai_;
    Machine machine_;
    Ai_out ai_out_;

    void setAiInput(Ai_input &ai_input,const std::vector<std::string> &buf_string,ConfigTCP &configTCP);
    int ai_fcr(const Ai_input &ai_input);
    Card changeCardType(std::string &card_string,ConfigTCP &configTCP);
};
















#endif //POKER_AI_MYGAME_H

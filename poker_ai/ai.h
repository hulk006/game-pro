//
// Created by yh on 17-6-22.
//

#ifndef POKER_AI_AI_H
#define POKER_AI_AI_H

#include "card.h"
#include "hand.h"
#include "machine.h"
#include <stdio.h>
#include <cstdlib>

class Ai
{
public:
    Ai();
    ~Ai();
    bool check_;
    int bet_;
    int game_pool_;
    int chip_;//剩余筹码
    int com_size_;//公牌张数<= 5
    int simuate_num_;
    float RR_;
    float HS_;
    Cards hole_com_cards_;//手牌和公牌
    Cards com_cards_;//公牌
    Cards hole_cards_;//手牌
    Hand  hand_;

    void init();
    //发手牌
    void setBet(const int &bet);
    void setGamePool(const int game_pool);
    void setHole(const Card & c1,const Card & c2);
    //第一次三张公牌
    void setFlop(const Card & c1,const Card & c2,const Card & c3);
    //第四张公牌
    void setTurn(const Card & c);
    //第五张公牌
    void setRiver(const Card & c);
    //下注分析 fold call raise
    int Ai_FCR(int &bet,int &gamepool,Machine &machine);
    float getStrength(Machine &machine);
    bool simulate(Machine &machine );
    //分析手牌
    void dealover();
//some get  value func
    int getChip();
    int getLevel();
    int getFinalValue();

    //for my test
    void blind(const int &bet);
    void take(const int &bet);
    void call(const int &bet);
    void showHole();
    void showComCards();
    void showChip();





};





#endif //POKER_AI_AI_H

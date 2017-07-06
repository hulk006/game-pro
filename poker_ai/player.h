//
// Created by yh on 17-6-22.
//

#ifndef POKER_AI_PLAYER_H
#define POKER_AI_PLAYER_H

#include "hand.h"
#include "card.h"
#include <iostream>

class Player
{
public:
    Player();
    ~Player();

    Hand hand_;
    int chip_;
    Cards hole_cards_;

    void init();

    void setHole(const Card & c1,const Card & c2);
    //第一次三张公牌
    void setFlop(const Card & c1,const Card & c2,const Card & c3);
    //第四张公牌
    void setTurn(const Card & c);
    //第五张公牌
    void setRiver(const Card & c);

    void dealOver();


    //for my test
    void blind(const int &bet);
    void take(const int &bet);
    void call(const int &bet);

    void showChip();
    void showHole();

    int getChip();
    int getLevel();
    int getFinalValue();






};
#endif //POKER_AI_PLAYER_H

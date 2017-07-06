//
// Created by yh on 17-6-21.
//

#ifndef POKER_AI_HAND_H
#define POKER_AI_HAND_H

#include "card.h"

const int SUIT_SHIFT[4] = {0, 3, 6, 9};
//change the 15872 to 15873,32 to 8223
const int STRAIGHT_VALUE[10] = {15873, 7936, 3968, 1984, 992, 496, 248, 124, 62, 8223 };
class Hand
{
public:
    Hand();
    ~Hand();
    bool initilized_;//是否初始化

    int flush_;//同花
    int straight_[4];//顺子，判断同花顺
    int hand_value_;//顺子
    int hand_size_;//自己牌张数
    int count_[14];//每张牌有几张，用来判断对子，葫芦，炸弹
    int level_;//牌的大小
    int final_value_;

    Cards hole_com_cards_;//手牌和公牌
    Cards hole_cards_;//手牌

    void initHand();
    void setCard(const Card &card);
    void ananlyzeHand();
    void ananlyzeCard(const Card &c);
    //for test
    void showHole();
};







#endif //POKER_AI_HAND_H

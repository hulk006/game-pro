//
// Created by yh on 17-6-21.
//

#ifndef POKER_AI_CARD_H
#define POKER_AI_CARD_H

#include <iostream>
#include <vector>

const int SUIT_SIZE = 4;
const int CARD_RANK =13;
const std::string SUIT_NAME[4] = {"Spade", "Heart", "Club", "Diamond" };
const std::string RANK_NAME[] ={"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
const std::string SUIT_CHINESE_NAME[] = {"黑桃", "红桃", "梅花", "方块"};

struct Card
{
    int suit_;//程序统一标准：0是黑桃、1是红桃、2是梅花、3是方片
    int value_;//0代表‘牌2’、1代表‘牌3’...more
    int show_time_ ;//发牌的顺序，用来排序sort
};
class Cards
{
public:
    Cards();
    ~Cards();
    std::vector<Card> cards_;
    void cardSort();//对牌进行排序，从小到大
    int turnToValue();//将手牌转换为值,每一副牌的大小唯一

private:
    int cardsLength(const std::vector<Card> &cards);
    void cardsSwap(int i,int j);


};





















#endif //POKER_AI_CARD_H

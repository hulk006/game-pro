//
// Created by yh on 17-6-22.
//
#include "machine.h"
#include <stdlib.h>
#include <vector>
#include "ctime"

Machine::Machine()
{
    initilized_ = false;
    top_index_ = 0;
}

Machine::~Machine() {}

/*
初始化牌组
对于花色：0代表黑桃、1代表红桃、2代表梅花、3代表方块
对于值：0代表two，1代表three ... 12代表A
*/
void Machine::init()
{
    cards_.cards_.clear();
    for (int i = 0; i < SUIT_SIZE; ++i)
    {
        for (int j = 0; j < CARD_RANK; ++j)
        {
            Card c;
            c.value_ = j;
            c.suit_ = i;
            c.show_time_ = 0;
            cards_.cards_.push_back(c);
        }
    }
    size_t len = cards_.cards_.size();
    top_index_ = 0;
    initilized_ = true;
}

/*
洗牌！！游戏每次开始时候调用，允许多次调用。
随机序列生成的逻辑是这样的：
从后往前，N个数为例。
先生成一0~~N-1的随机数i，然后置换i和N之间的位置
同理处理N-1....
*/
void Machine::shuffleCards()
{
    if(initilized_ == false)
    {
        //error
        std::cout << "error! you must init Machine first"<<std::endl;
    }
    else
    {
       // 要取得[a,b]的随机整数，使用(rand() % (b-a+1))+ a （结果值含a和b）
        int  card_num = SUIT_SIZE*CARD_RANK;
        int  rand_int = 0;
        for (int i = card_num - 1; i > 0 ; i--)
        {
            i;
            srand(unsigned(time(0)));
            rand_int = rand()%i;//[0 - 51]
            swapCard(i,rand_int);
        }
        top_index_ = 0;
    }
}



Card Machine::dealCard()
{
    Card c = cards_.cards_[top_index_];
    top_index_++;
    if(top_index_ == SUIT_SIZE*CARD_RANK)
    {
        shuffleCards();
    }
    return c;
}

void Machine::swapCard(int i, int j)
{
    Card temp;
    temp.suit_ = cards_.cards_[i].suit_;
    temp.value_ = cards_.cards_[i].value_;
    cards_.cards_[i] = cards_.cards_[j];
    cards_.cards_[j] = temp;
}
void Machine::showCards() {
    for (int i = 0; i < SUIT_SIZE*CARD_RANK ; ++i)
    {
        std::cout <<SUIT_NAME[cards_.cards_[i].suit_]<<" " << cards_.cards_[i].value_<<std::endl;
    }
}
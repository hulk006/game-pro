//
// Created by yh on 17-6-22.
//
#include "machine.h"
#include <stdlib.h>
#include <vector>
#include "ctime"
#include <sys/timeb.h>

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
void Machine::init(const Cards hole_com_cards)
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
    know_cards_ = hole_com_cards;
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
        int len = cards_.cards_.size();
        removeKonwnCards();//把目前已知的牌删除
        int len1 = cards_.cards_.size();

        int konwn_cards_num = know_cards_.cards_.size();
       // 要取得[a,b]的随机整数，使用(rand() % (b-a+1))+ a （结果值含a和b）
        int  card_num = SUIT_SIZE*CARD_RANK -konwn_cards_num;
        int  rand_int = 0;
        for (int i = card_num - 1; i > 0 ; i--)
        {
            //struct timeb timeSeed;
            //ftime(&timeSeed);
            //double time = timeSeed.time * 1000 + timeSeed.millitm;
            // srand(timeSeed.time * 1000 + timeSeed.millitm);  // milli time
            rand_int = rand()%i;//[0 - 51-konwn_cards_num]
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
void Machine::showCards()
{
    for (int i = 0; i < SUIT_SIZE*CARD_RANK ; ++i)
    {
        std::cout <<SUIT_NAME[cards_.cards_[i].suit_]<<" " << cards_.cards_[i].value_<<std::endl;
    }
}
void Machine::removeKonwnCards()//用于计算胜率的时候，把已知的牌移除，使得胜率计算更加准确
{
    if(know_cards_.cards_.size()>7)
    {
        std::cout<<"error occurs,int the removeKonwnCards() "<<std::endl;
    }
    std::vector<Card>::iterator it ;

    for(int i = 0;i< know_cards_.cards_.size(); ++i)
    {
        int known_card_rank = convertCardToNum(know_cards_.cards_[i]);
        it = cards_.cards_.begin();
        while (it != cards_.cards_.end())
        {
            int cards_rank = convertCardToNum( *it );
            if(cards_rank == known_card_rank)
            {
                cards_.cards_.erase(it);
                it++;
                break;
            } else
            {
                it++;
            }

        }
    }
}

int  Machine::convertCardToNum(const Card c)
{
    int card_rank = c.suit_ * 13 + c.value_;
    return card_rank;
}
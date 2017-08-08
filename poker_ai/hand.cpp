//
// Created by yh on 17-6-21.
//

#include <iostream>
#include "hand.h"


Hand::Hand()
{
    flush_ = 0;//同花顺
    hand_value_ = 0;
    hand_size_ = 0;
    initilized_ = true;
    level_ = -1;
    final_value_ = -1;
    for(int i=0;i<14;++i)
    {
        count_[i] = 0;
    }
    //count_[14] = {0};
    for(int i=0;i<4;++i)
    {
        straight_[i] = 0;
    }

}
Hand::~Hand()
{}
void Hand::initHand()
{
    flush_ = 0;//同花顺
    hand_value_ = 0;
    hand_size_ = 0;
    initilized_ = true;
    level_ = 1;
    final_value_ = 0;
    for(int i=0;i<14;++i)
    {
        count_[i] = 0;
    }
    //count_[14] = {0};
    for(int i=0;i<4;++i)
    {
        straight_[i] = 0;
    }
    hole_cards_.cards_.clear();
    hole_com_cards_.cards_.clear();
}

void Hand::setCard(const Card &card)
{
    if(initilized_== false)
    {
        //return error
        std::cout << "Hand must init first"<<std::endl;
    }

    if(hand_size_ < 2)
    {
        Card card1;
        card1.suit_ = card.suit_;
        card1.value_ = card.value_;
        hole_cards_.cards_.push_back(card1);
    }
    if(hand_size_ == 7)
    {
        //game end
        std::cout << "fter a game, you should init Hand again"<<std::endl;
    }
    hole_com_cards_.cards_.push_back(card);
    hand_size_++;
    ananlyzeCard(card);

}

void Hand::ananlyzeCard(const Card &c)
{
    flush_ += 1 << uint(SUIT_SHIFT[c.suit_]);
    straight_[c.suit_] |= 1 << uint(c.value_ + 1);
    hand_value_ |= 1 << uint(c.value_ + 1);
   // if(c.value_ == 12)
    //{
      //  straight_[c.suit_] |= 1;
      //  hand_value_ |= 1;
    //}

    count_[c.value_]++;
}

void Hand::ananlyzeHand()
{
    if(initilized_== false)
    {
        //return error
        std::cout << "Hand must init first"<<std::endl;
    }
    //首先对手牌排序
    hole_com_cards_.cardSort();
    int temp = hole_com_cards_.turnToValue();
    bool flag = false;

    //判断高牌
    level_ = 1;
    final_value_ = temp;

    //一对
    for (int i = 0; i < CARD_RANK; ++i)
    {
        if(count_[i] == 2)
        {
            level_ = 2;
            final_value_ = temp;
            break;
        }
    }

    //两对
    for (int i = 0; i < CARD_RANK; ++i)
    {
        if(count_[i] == 2)
        {
            for (int j = i+1; j < CARD_RANK; ++j)
            {
                if(count_[j] == 2)
                {
                    level_ = 3;
                    final_value_ = temp;
                    flag = true;
                    break;
                }
            }

            if(flag)
            {
                flag = false;
                break;
            }
        }
    }
   
    //三条
    for (int i = 0; i < CARD_RANK; ++i)
    {
        int num = 0;
        if(count_[i] == 3)
        {
            level_ = 4;
            final_value_ = temp;
            break;
        }
    }

    //顺子
    for (int i = 0; i < 10; ++i)
    {
        int aaa=hand_value_&STRAIGHT_VALUE[i];

        if((hand_value_&STRAIGHT_VALUE[i]) == STRAIGHT_VALUE[i])
        {
            level_ = 5;
            final_value_ = 10 - i +4;//同上，表示顺子的大小
        }

    }

    //同花
    for (int i = 0; i < SUIT_SIZE; ++i)
    {
        int flush_flag = (flush_>>uint(SUIT_SHIFT[i])) & 5;
        if(flush_flag == 5)
        {
            level_ = 6;
            final_value_ = straight_[i];
            break;
        }
    }

    //葫芦
    for (int i = 0; i < CARD_RANK ; ++i)
    {
        if(count_[i] == 3)
        {
            for (int j = 0; j <CARD_RANK; ++j)
            {
                if(count_[j] == 2)
                {
                    level_ = 7;
                    final_value_ = temp;
                    flag = true;
                    break;
                }
            }

            if(flag)
            {
                flag = false;
                break;
            }
        }

    }

    //四条
    for (int i = 0; i < CARD_RANK ; ++i)
    {
        if(count_[i] == 4)
        {
            level_ = 8;
            final_value_ = temp;
            break;
        }
    }

    //判断同花顺
    for (int i = 0; i < SUIT_SIZE ; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if(straight_[i] == STRAIGHT_VALUE[j] )
            {
                level_ = 9;//同花顺
                final_value_ = 10 - j + 4;//表示顺子的大小
                flag = true;
                break;
            }
        }
        if(flag)
        {
            flag = false;
            break;
        }
    }

    //判断皇家同花顺
    for (int i = 0; i < SUIT_SIZE ; ++i)
    {
        if(straight_[i] == STRAIGHT_VALUE[0] )
        {
            level_ = 10;
            flag = true;
        }
        if(flag)
        {
            flag = false;
            break;
        }
    }
}

void Hand::showHole()
{
    std::cout<<"电脑手牌为"<<std::endl;
    for (int i = 0; i < hole_cards_.cards_.size(); ++i)
    {
     std::cout<<SUIT_CHINESE_NAME[hole_cards_.cards_[i].suit_]<<RANK_NAME[hole_cards_.cards_[i].value_]<< " ";
    }
    std::cout<<std::endl;
}
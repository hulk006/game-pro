//
// Created by yh on 17-6-22.
//
//玩家的信息的
#include "player.h"

Player::Player()
{
    chip_ = 5000;

}
Player::~Player()
{

}


void Player::init()
{
    hand_.initHand();
}

void Player::setHole(const Card & c1,const Card & c2)
{
    if(hole_cards_.cards_.size() == 0)
    {
        //手牌
        hole_cards_.cards_.push_back(c1);
        hole_cards_.cards_.push_back(c2);
        //hand的手牌
        hand_.setCard(c1);
        hand_.setCard(c2);

    } else
    {
        std::cout << "error ! play need init" << std::endl;
    }
}
//第一次三张公牌
void Player::setFlop(const Card & c1,const Card & c2,const Card & c3)
{
    if(hand_.hole_com_cards_.cards_.size() <= 2)
    {
        //hand的手牌
        hand_.setCard(c1);
        hand_.setCard(c2);
        hand_.setCard(c3);
    } else
    {
        std::cout << "error ! player need init" << std::endl;
    }
}
//第四张公牌
void Player::setTurn(const Card & c)
{
    hand_.setCard(c);
}
//第五张公牌
void Player::setRiver(const Card & c)
{
    hand_.setCard(c);
}

void Player::dealOver()
{
    hand_.ananlyzeHand();
}


//for my test
void Player::blind(const int &bet)
{
    chip_ -= bet;
}
void Player::take(const int &bet)
{
    chip_ += bet;
}
void Player::call(const int &bet)
{
    chip_ -= bet;
}

void Player::showChip()
{
    std::cout <<"你的本金还剩:" <<chip_<<std::endl;
}
void Player::showHole()
{
    std::cout <<"你的手牌为:"
              <<SUIT_CHINESE_NAME[hole_cards_.cards_[0].suit_]<<" "<< RANK_NAME[hole_cards_.cards_[0].value_]<<" "
              <<SUIT_CHINESE_NAME[hole_cards_.cards_[1].suit_]<<" "<< RANK_NAME[hole_cards_.cards_[1].value_]
              <<std::endl;
}


int Player::getChip()
{
    return chip_;
}
int Player::getLevel()
{
    return hand_.level_;
}
int Player::getFinalValue()
{
    return hand_.final_value_;
}
//
// Created by yh on 17-6-22.
//
#include "ai.h"

Ai::Ai()
{
    bet_ = 100;//最小下注数；
    game_pool_ = 0;
    chip_ = 5000;//ai 初始的本金
    com_size_ = 0;//初始化时时候没有公牌手牌
    simuate_num_ = 1000;//仿真1000次
    com_cards_.cards_.clear();
    hole_com_cards_.cards_.clear();
    hole_cards_.cards_.clear();
    RR_ = 1;
    HS_ = 0.5;

}

Ai::~Ai() {}

//一轮游戏ai需要冲洗初始化
void Ai::init()
{
    com_size_ = 0;//初始化时时候没有公牌手牌
    hand_.initHand();
    com_cards_.cards_.clear();
    hole_com_cards_.cards_.clear();
    hole_cards_.cards_.clear();
}

void Ai::setBet(const int &bet)
{
    bet_ = bet;
}

void Ai::setGamePool(const int game_pool)
{
    game_pool_ = game_pool;
}

void Ai::setHole(const Card &c1, const Card &c2)
{
    if(hole_cards_.cards_.size() == 0)
    {
        //手牌
        hole_cards_.cards_.push_back(c1);
        hole_cards_.cards_.push_back(c2);
        //hand的手牌
        hand_.setCard(c1);
        hand_.setCard(c2);
        //手牌和公牌
        hole_com_cards_.cards_.push_back(c1);
        hole_com_cards_.cards_.push_back(c2);

    } else
    {
        std::cout << "error ! AI need init" << std::endl;
    }
}

void Ai::setFlop(const Card &c1, const Card &c2, const Card &c3)
{
    if(com_size_ == 0)
    {
        //hand的手牌
        hand_.setCard(c1);
        hand_.setCard(c2);
        hand_.setCard(c3);

        //手牌和公牌
        hole_com_cards_.cards_.push_back(c1);
        hole_com_cards_.cards_.push_back(c2);
        hole_com_cards_.cards_.push_back(c3);
        //
        com_cards_.cards_.push_back(c1);
        com_cards_.cards_.push_back(c2);
        com_cards_.cards_.push_back(c3);
        com_size_ = 3;

    } else
    {
        std::cout << "error ! AI need init" << std::endl;
    }
}

void Ai::setTurn(const Card &c)
{
    if(com_size_ == 3)
    {
        com_size_++;
        //hand的手牌
        hand_.setCard(c);
        //手牌和公牌
        hole_com_cards_.cards_.push_back(c);
        //
        com_cards_.cards_.push_back(c);

    }
    else
    {
        std::cout << "error !before the turn ,ai need 3 community cards" << std::endl;
    }
}

void Ai::setRiver(const Card &c)
{
    if(com_size_ == 4)
    {
        com_size_++;
        //hand的手牌
        hand_.setCard(c);
        //手牌和公牌
        hole_com_cards_.cards_.push_back(c);
        //
        com_cards_.cards_.push_back(c);
    }
    else
    {
        std::cout << "error !before the turn ,ai need 4 community cards" << std::endl;
    }
}

void Ai::dealover()
{
    hand_.ananlyzeHand();
}
//可以改变一些阈值来改变电脑的聪明程度，需要接下来调试
int Ai::Ai_FCR(int &bet,int &gamepool,Machine &machine)
{
    float RR = 0;
    machine.init(hole_com_cards_);
    float hand_strength = getStrength(machine);
    std::cout<<"hand_strength ="<<hand_strength<<std::endl;
    HS_ =hand_strength;
    float hs_small = 0.34;
    float hs_big = 0.65;
    int final_bet = bet;
    //特殊情况 ，如果剩余筹码很少，并且手牌较小，就丢掉牌
    if (chip_ - bet < 400 && hand_strength < hs_small)
    {
        RR_ = 0.1;
        final_bet = 0;
        return final_bet;//fold
    }
    //特殊情况 很大的几率获胜
    if(hand_strength >= 0.8)
    {
        std::cout<< " RR = "<<RR<<std::endl;
        RR_ = 10;
        final_bet = chip_;
        return final_bet;//all in
    }

    //第一轮 TO DO
    if(bet == 0 || gamepool == 3*10 ||gamepool == 3*50||gamepool == 3*100||gamepool == 3*200 )
    {
        if(hand_strength < 0.2)
        {
            final_bet = 0;
            return final_bet;//fold
        }
        else
        {
            if(bet > 5*gamepool)
            {
                if(hand_strength > (0.6))
                {
                    final_bet = bet;
                    return final_bet;
                } else
                {
                    final_bet = 0;
                    return final_bet;//fold
                }
            }
            else
            {
                std::cout<<" 111"<<std::endl;
                RR = 1.3;
                RR -= hand_strength;
                RR_ = RR;
                std::cout<< " RR = "<<RR<<std::endl;
                final_bet = float(hand_strength)*gamepool/float(RR);
                return final_bet;
            }
        }
    }
    //不是第一轮
    else
    {
        //首先计算回报率
        //TO DO
        RR = hand_strength * (float ( gamepool)) / float(bet);
        RR_ = RR;
        HS_ = hand_strength;
        std::cout<< " RR = "<<RR<<std::endl;
        srand((int)time(NULL));
        int p = std::rand()%100;//随机生成0到100
        std::cout<< " p = "<<p<<std::endl;
        if(hand_strength <= hs_small)
        {
            if(p>70)//
            {
                final_bet =bet;
                return final_bet;
            } else//这是一个大概率事件，如果回报率小于hs_small，就fold
            {
                final_bet = 0;
                return final_bet;//fold
            }
        }
       else if(hand_strength > hs_small && hand_strength < hs_big)
        {
            if(RR < 0.7 )//处于容易输钱的状态
            {
                if (p >= 65)//if (p >= 30)这是一个大概率事件，如果回报率小于0.8，就fold.本来是30改成70为了减少ai丢牌的概率
                {
                    final_bet = 0;
                    return final_bet;//fold
                }
                else if(p < 60 && p>1)//小概率 蒙人的；
                {
                    final_bet = bet;
                    return final_bet;
                }
                else //唬人的
                {
                    final_bet = 4*bet;
                    return final_bet;
                }

            }
            if(RR>=0.7 && RR < 1 )//处于容易输钱的状态
            {
                if (p > 95)//这是一个大概率事件，如果回报率小于0.8，就fold
                {
                    final_bet = 0;
                    return final_bet;//fold
                } else//小概率 蒙人的；
                {
                    final_bet = bet;
                    return final_bet;//call
                }
            }
            else if(RR>= 1 && RR<1.5)//处于容易赢钱的状态
            {
                if(p >= 99)//很小的概率fold
                {
                    final_bet = 0;
                    return final_bet;//fold
                }
                else if(p < 60)
                {
                    final_bet = bet;
                    return final_bet;//call
                }
                else
                {
                    final_bet = 2*bet;
                    return final_bet;//raise
                }
            }
            else if (RR>=1.5 && RR<1.8)// 不会fold
            {
                if(p > 50)
                {
                    final_bet = bet;
                    return final_bet;//call
                }
                else
                {
                    final_bet = 2*bet;
                    return final_bet;//raise
                }
            }
            else if (RR >= 1.8)// 不会fold
            {
                if(p > 70)
                {
                    final_bet = bet;
                    return final_bet;//call
                }
                else
                {
                    final_bet = 2*bet;
                    return final_bet;//raise
                }
            }

        }
        else if(hand_strength > hs_big )
        {
            if(p>40)
            {
                final_bet = 2*bet;
                return final_bet;//raise
            }
            else
            {
                final_bet = 4*bet;
                return final_bet;//raise
            }

        }

    }

    return final_bet;

}

float Ai::getStrength(Machine &machine)
{
    int win_times = 0;
    for(int i = 0; i < simuate_num_ ; i++)//仿真100，循环进不去
    {
        bool is_win = simulate(machine);
        //std::cout<< "win" << win_times << " "<<is_win<< std::endl;
        if(is_win)
        {
            win_times++;
        }
        else
        {
            continue;
        }

    }
    //std::cout<< "win" << win_times << std::endl;
    float hs = float(win_times)/float(simuate_num_);
    return hs;

}

bool Ai::simulate(Machine &machine)
{
    Hand hand_self,hand_opp;
    hand_self.initHand();
    hand_opp.initHand();
    machine.init(hole_com_cards_);//每一次洗牌前都要初始化
    machine.shuffleCards();

    //把ai的手牌给模拟器
    hand_self.setCard(hole_cards_.cards_[0]);
    hand_self.setCard(hole_cards_.cards_[1]);
    //给模拟的对手发牌
    hand_opp.setCard(machine.dealCard());
    hand_opp.setCard(machine.dealCard());
    //如果有公牌
    for (int j = 0; j < com_size_; ++j)
    {
        hand_self.setCard(com_cards_.cards_[j]);
        hand_opp.setCard(com_cards_.cards_[j]);
    }
    //
    for (int j = 0; j < 5- com_size_; ++j)
    {
        Card card = machine.dealCard();
        hand_self.setCard(card);
        hand_opp.setCard(card);
    }
    //分析哪个胜利
    hand_self.ananlyzeHand();
    if(com_size_>=3)
    {
        hand_.level_ = hand_self.level_;
        hand_.final_value_ = hand_self.final_value_;
    }
    hand_opp.ananlyzeHand();
    //std::cout<< hand_self.level_ <<"?"<<hand_opp.level_<<std::endl;
    //比较
    if(hand_self.level_ > hand_opp.level_)
    {
        return true;
    }
    else if (hand_self.level_ == hand_opp.level_
             && hand_self.final_value_ >hand_opp.final_value_)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void Ai::blind(const int &bet)
{
    chip_ -= bet;
}

void Ai::take(const int &bet)
{
    chip_ += bet;
}

void Ai::call(const int &bet)
{
    chip_ -= bet;
}

int Ai::getChip()
{
    return chip_;
}

int Ai::getLevel()
{
    return hand_.level_;

}

int Ai::getFinalValue()
{
    return hand_.final_value_;
}

void Ai::showHole()
{
    hand_.showHole();
}
void Ai::showComCards()
{
    std::cout << "目前场上的公牌为："<<std::endl;
    for (int i = 0; i < com_size_ ; ++i)
    {
        std::cout << SUIT_CHINESE_NAME[com_cards_.cards_[i].suit_]
                  << RANK_NAME[com_cards_.cards_[i].value_]
                  <<std::endl;
    }

}
void Ai::showChip()
{
    std::cout << "电脑的本金还剩余：" <<chip_<<std::endl;
}

//
// Created by yh on 17-6-22.
//


#include "mygame.h"
Mygame::Mygame()
{
}
Mygame::~Mygame() {}


void Mygame::setAiInput(Ai_input &ai_input,const std::vector<std::string> &buf_string,ConfigTCP &configTCP)
{
    int start = 2;
    ai_input.hole_cards_.cards_.clear();
    ai_input.hole_cards_ = configTCP.ai_input_.hole_cards_;

    ai_input.com_cards_.cards_.clear();
    ai_input.com_cards_ = configTCP.ai_input_.com_cards_;


    //bet
    std::vector<std::string> bet_string;
    splitString(buf_string[start+2],",",bet_string);
    ai_input.current_bet_ = std::stoi(bet_string[1]);
    //gamepool
    std::vector<std::string> gamepool_string;
    splitString(buf_string[start+3],",",gamepool_string);
    ai_input.game_pool_ = std::stoi(gamepool_string[1]);
    //chip
    std::vector<std::string> chip_string;
    splitString(buf_string[start+4],",",chip_string);
    ai_input.chip = std::stoi(chip_string[1]);
    //check
    std::vector<std::string> check_string;
    splitString(buf_string[start+5],",",check_string);
    int check =std::stoi(check_string[1]);

    if ( std::stoi(check_string[1])==1)
    {
        ai_input.check_ = true;
    } else{
        ai_input.check_ = false;
    }

}

int Mygame::ai_fcr(const Ai_input &ai_input)
{
    ai_.setHole(ai_input.hole_cards_.cards_[0],ai_input.hole_cards_.cards_[1]);
    if(ai_input.com_cards_.cards_.size()>=3)
    {
        ai_.setFlop(ai_input.com_cards_.cards_[0],ai_input.com_cards_.cards_[1],ai_input.com_cards_.cards_[2]);
    }
    if(ai_input.com_cards_.cards_.size()>=4)
    {
        ai_.setTurn(ai_input.com_cards_.cards_[3]);
    }
    if(ai_input.com_cards_.cards_.size()>=5)
    {
        ai_.setRiver(ai_input.com_cards_.cards_[4]);
    }

    ai_.chip_ = ai_input.chip;
    ai_.game_pool_ = ai_input.game_pool_;
    ai_.bet_ = ai_input.current_bet_;
    ai_.check_ = ai_input.check_;
    int fcrca = 0;
    int fcr = 0;
    fcr = ai_.Ai_FCR(ai_.bet_,ai_.game_pool_,machine_);//得到一个电脑的下注数
    std::cout<<"fcr"<<fcr<<std::endl;
    float allin_threhold = 0.84;//经验值，

    if(ai_.com_cards_.cards_.size() < 1||ai_.hole_com_cards_.cards_.size() == 2 )//pre flop没有工牌 只有看牌和跟住操作
    {
        if(ai_.check_ == true) //可以让牌
        {
            if(fcr >= 0 && fcr < 2*ai_.bet_)//电脑fold
            {
                fcrca = 3;
            }
            else //跟住
            {
                fcrca = 1;
            }
        }
        else//不可以看牌
        {
            if(fcr == 0)//电脑fold
            {
                fcrca = 0;
            }
            else //跟注
            {

                if(ai_.HS_ > allin_threhold)
                {
                    int p = rand()%100;//随机生成0到100
                    if (p > 90)//allin 的几率 10%
                    {
                        fcrca = 4;
                    }
                    else
                    {
                        fcrca = 1;
                    }
                }
		else
		{fcrca = 1;
		}
            }
        }
    }
    else//有公牌的阶段，正常进行
    {
        if(ai_.check_ == true) //可以让牌
        {
            if(fcr == 0)//电脑本来fold，但是可以看牌啊
            {
                fcrca = 3;//看牌
            }
            else if(fcr > 0 && fcr < 2*ai_.bet_)//跟注
            {
                fcrca = 1;
            }
            else if(fcr >= 2*ai_.bet_ && fcr < ai_.getChip())//加注或者all in
            {
                int p = rand()%100;//随机生成0到100
                if (p>95)//allin 的几率 10%
                {
                    fcrca = 4;
                } else
                {
                    fcrca = 2;
                }
            }
            else if(fcr >= ai_.getChip())//电脑all in
            {
                if( ai_.HS_ > allin_threhold)
                {
                    fcr = ai_.getChip();
                    int p = rand()%100;//随机生成0到100
                    if (p > 40)//allin 的几率 10%
                    {
                        fcrca = 4;
                    }
                    else
                    {
                        fcrca = 2;
                    }
                }
                else
                {
                    fcrca =  2;//raise
                }
            }
            else //默认看牌
            {
                fcrca = 3;//check
            }

        }
        else//不能让牌的情况
        {
            if(fcr == 0)//fold
            {
                fcrca = 0;
            }
            else if(fcr > 0 && fcr < 2*ai_.bet_)//跟注
            {
                std::cout<<"电脑跟注"<<std::endl;
                fcrca = 1;
            }
            else if(fcr >= 2*ai_.bet_ && fcr < ai_.getChip())//加注
            {
                std::cout<<"电脑加注"<<std::endl;
                fcrca =  2;
            }
            else if(fcr >= ai_.getChip())//电脑all in
            {
                if( ai_.HS_ > allin_threhold) //胜率很大
                {
                    fcr = ai_.getChip();
                    int p = rand()%100;//随机生成0到100
                    if (p > 40)//allin 的几率 10%
                    {
                        fcrca = 4;
                    }
                    else
                    {
                        fcrca = 2;
                    }
                }
                else
                {
                    fcrca =  2;//raise
                }
            }
            else//默认跟注
            {
                fcrca = 1;
            }
        }
    }

    ai_out_.fcr_bet_ = fcr;
    ai_out_.fcrca_ = fcrca;
    ai_out_.RR_ =ai_.RR_;
    ai_out_.strength_hand_ = ai_.HS_;
    return fcrca;
}


Card  Mygame::changeCardType(std::string &card_string,ConfigTCP &configTCP)
{
    Card C;
    std::cout<<card_string<<std::endl;
    char value_raw = card_string[0];//
    char suit_raw = card_string[1];//
    switch ((int)value_raw)
    {
        case 84:
            C.value_ = 10-2;
            break;
        case 74:
            C.value_ = 11-2;
            break;
        case 81:
            C.value_ = 12-2;
            break;
        case 75:
            C.value_ = 13-2;
            break;
        case 65:
            C.value_ = 14-2;
            break;
        default:
            C.value_ = (int)(card_string[0])- 50;
            break;
    }

    if( C.value_<0 ||  C.value_ > 12)
    {
        std::cout<<"error input card value (2 ~ TJQKA)!!!!!!"<<std::endl;
        configTCP.errorSend();
        exit(0);//退出重启
    }

    switch ((int)suit_raw)
    {
        case 68://D
            C.suit_ = 3;
            break;
        case 67://C
            C.suit_ = 2;
            break;
        case 72://H
            C.suit_ = 1;
            break;
        case 83://S
            C.suit_ = 0;
            break;
        default:
            std::cout<<"error in suit type change!!!!!!"<<std::endl;
            configTCP.errorSend();
            exit(0);//退出重启
            break;
    }
    return C;
}



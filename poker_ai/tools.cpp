//
// Created by yh on 17-8-7.
//

#include "tools.h"
void splitString( const std::string &input,char *split,std::vector<std::string> &output)
{
    output.clear();
    std::vector<std::string> input_split;
    char temp[1024];
    strcpy(temp,input.c_str());
    char *tmpStr = strtok(temp, split);
    while(tmpStr!=NULL)
    {
        input_split.push_back(std::string(tmpStr));
        tmpStr = strtok(NULL, split);
    }
    output = input_split;
}

bool stringToCard(const std::string &card_string, Card &card)//转化为一张牌
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
        return 0;
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
        default://其他花色就直接返回0，错误
            std::cout<<"error in suit type change!!!!!!"<<std::endl;
            return 0;
            break;
    }
    card = C;
    return true;
}

bool stringToCards(const std::string &cards_string, Cards &cards)//把多张牌转化为一手牌
{
    //hole
    if(cards_string.find("hole") != std::string::npos)//如果可以找到hole 那就是hole
    {
        cards.cards_.clear();
        Card C[2];
        std::vector<std::string> hole_string;
        splitString(cards_string,",",hole_string);//分割手牌的字符
        if(stringToCard(hole_string[1],C[0])&&
           stringToCard(hole_string[2],C[1]))
        {
            cards.cards_.push_back(C[0]);
            cards.cards_.push_back(C[1]);
        }
        else
        {
            return false;
        }
    }//com
    else if(cards_string.find("com") != std::string::npos)
    {
        cards.cards_.clear();
        std::vector<std::string> com_string;
        splitString(cards_string,",",com_string);//分割手牌的字符
        if(com_string.size()>=1 && com_string.size() <= 6)
        {
            for(int i = 1;i<com_string.size();++i)
            {
                Card C;
                if(stringToCard(com_string[i],C))
                {
                    cards.cards_.push_back(C);
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            //cards.cards_.clear();//不需要
        }
    }
    else//不能匹配公牌和手牌
    {
        return false;
    }
    return true;
}

bool stringToAiInput(const std::vector<std::string> &buf_split,Ai_input &ai_input)
{
    std::map<std::string ,std::string> recv_buf;
    for(int i = 4;i < buf_split.size()-1; ++i)
    {
        std::vector<std::string> split ;

        splitString(buf_split[i],",",split);
        std::string a= buf_split[i];
        std::string b=  split[0];
        std::string c=  split[1];

        if(split.size() == 2 && std::stoi(split[1])>= 0)//长度为2
        {
            recv_buf.insert( std::pair<std::string, std::string>(split[0],split[1]) );
            std::string d = recv_buf[b];

            i;
        }
        else
        {
            return false;
        }
    }
    std::string mm = recv_buf["game_pool_"];
    int ma = std::stoi(mm);
    if( std::stoi(recv_buf["check_"])>0)
    {
        ai_input.check_ = true;
    } else
    {
        ai_input.check_ = false;
    }
   // ai_input.check_ =  std::stoi(recv_buf["check"]);
    ai_input.chip = std::stoi(recv_buf["chip"]);
    ai_input.current_bet_ = std::stoi(recv_buf["current_bet"]);
    ai_input.game_pool_ = std::stoi(recv_buf["game_pool_"]);
}
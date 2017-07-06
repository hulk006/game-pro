//
// Created by yh on 17-6-21.
//

#include "card.h"
Cards::Cards()
{

}
Cards::~Cards() {

}


int Cards::cardsLength(const std::vector<Card> &cards)
{
    int len = cards.size();
    return len;
}

void Cards::cardsSwap(int i,int j)
{
    Card temp = cards_[i];
    cards_[i] = cards_[j];
    cards_[j] = temp;
}
void Cards::cardSort()
{
    if(cards_.size() == 0)
    {
        //error
        std::cout <<"there is no card! "<<std::endl;
    }
    else if (cards_.size()==1)
    {
        std::cout <<"there is only one card! "<<std::endl;
    }
    else
    {
        for (int i = 0; i < cards_.size(); ++i)
        {
            for (int j = i; j <cards_.size() ; ++j)
            {
                if(cards_[i].value_ > cards_[j].value_)
                {
                    cardsSwap(i, j);
                }
                else
                {
                    //do nothing
                }
            }
        }

    }

}

int Cards::turnToValue()
{
    //there is a problem the 10 need to change to 15;
    int res = 0;
    for (int i = 0; i < cards_.size(); ++i)
    {
      res *= 15;
      res += cards_[i].value_;
    }
    return res;
}
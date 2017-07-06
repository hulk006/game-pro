#include <iostream>

int main() {

    std::string card_string = "QD";
    char value_raw = card_string[0];//
    char suit_raw = card_string[1];//
    char *t="T",*j="J",*q="Q",*k="K",*a="A";
    int T=(int)*t,J=(int)*j,Q=(int)*q,K=(int)*k,A=(int)*a;
    int value;
    switch ((int)value_raw)
    {
        case 84:
            value = 10-2;
            break;
        case 74:
             value = 11-2;
           break;
        case 81:
             value = 12-2;
           break;
        case 75:
             value = 13-2;
          break;
        case 65:
             value = 14-2;
          break;
        default:
               value = (int) card_string[0]-50 ;
            break;
    }
    int te = 574&7936;
 int card[7] = {0,4,4,5,5,6,8};
        int res = 0;
        for (int i = 0; i < 7; ++i)
        {
            res *= 10;
            res += card[i];
        }
    int card1[7] = {0,4,4,5,5,6,8};
    int res1 = 0;
    for (int i = 0; i < 7; ++i)
    {
        res1 *= 10;
        res1 += card1[i];
    }


    //T;



    std::cout << "Hello, World!" << std::endl;
    return 0;
}
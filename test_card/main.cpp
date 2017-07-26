#include <iostream>

int main(int argc,char **argv)
{
    std::cout << "Hello, World!" << std::endl;
    for(int i = 0;i<argc;++i)
    {
        std::cout<<argc<<" "<<"input param is:"<<argv[i]<<std::endl;
    }

    return 0;
}
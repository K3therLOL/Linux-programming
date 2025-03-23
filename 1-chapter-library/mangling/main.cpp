#include <iostream>
#include "num.h"

void invite(void)
{
    std::cout << "There is your random num: ";
}

void end(void)
{
    std::cout << "Goodbye, love!\n";
}

int main()
{
    invite();
    get_num();
    end();
}

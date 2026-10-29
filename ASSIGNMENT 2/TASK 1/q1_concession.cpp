#include <iostream>

void applyByValue(long long fee, int percent)
{
    fee -= fee * percent / 100;
}

void applyConcession(long long& fee, int percent)
{
    fee -= fee * percent / 100;
}

long long applyAndReturn(long long fee, int percent)
{
    return fee - fee * percent / 100;
}

int main()
{
    long long hostelFee = 45000;
    applyByValue(hostelFee, 10);
    std::cout << "after call by value : " << hostelFee << '\n';
    applyConcession(hostelFee, 10);
    std::cout << "after call by reference: " << hostelFee << '\n';
    hostelFee = applyAndReturn(hostelFee, 10);
    std::cout << "after return-value design: " << hostelFee << '\n';
}

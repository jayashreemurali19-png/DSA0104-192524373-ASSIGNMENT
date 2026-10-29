#include <iostream>
#include <cstdlib>

class Time {
    int h, m;

public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};

inline int minutesBetween(const Time& a, const Time& b)
{
    int am = a.h * 60 + a.m;
    int bm = b.h * 60 + b.m;
    return std::abs(am - bm);
}

inline bool earlier(const Time& a, const Time& b)
{
    return a.h * 60 + a.m < b.h * 60 + b.m;
}

int main()
{
    Time departure(9, 45), arrival(13, 10);
    std::cout << minutesBetween(departure, arrival) << ' '
              << std::boolalpha << earlier(arrival, departure) << '\n';

    Time noonBefore(11, 55), noonAfter(12, 5), same(12, 5);
    std::cout << minutesBetween(noonBefore, noonAfter) << ' '
              << earlier(noonBefore, noonAfter) << ' '
              << earlier(same, noonAfter) << '\n';
}

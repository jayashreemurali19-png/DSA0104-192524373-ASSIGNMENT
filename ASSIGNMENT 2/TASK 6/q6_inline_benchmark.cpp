#include <chrono>
#include <iostream>
#include <cstdint>

struct PlainTime { int h, m; };

int differenceNoInline(const PlainTime& a, const PlainTime& b)
{
    int x = a.h * 60 + a.m, y = b.h * 60 + b.m;
    return x > y ? x - y : y - x;
}

inline int differenceInline(const PlainTime& a, const PlainTime& b)
{
    int x = a.h * 60 + a.m, y = b.h * 60 + b.m;
    return x > y ? x - y : y - x;
}

template <typename F>
long long run(F f)
{
    PlainTime a{9, 45}, b{13, 10};
    volatile std::uint64_t sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 10000000; ++i) sum += f(a, b);
    auto stop = std::chrono::steady_clock::now();
    std::cout << "checksum=" << sum << ' ';
    return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
}

int main()
{
    std::cout << "non-inline: " << run(differenceNoInline) << " us\n";
    std::cout << "inline:     " << run(differenceInline) << " us\n";
}

#include <iostream>
#include <string>

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
};

double charge(double weightKg, bool express = false, bool insured = false)
{
    double total = 40.0 + weightKg * 20.0;
    if (express) total += 30.0;
    if (insured) total += 15.0;
    return total;
}

double charge(int weightGrams)
{
    return 40.0 + (weightGrams / 1000.0) * 20.0;
}

double charge(const Parcel& p)
{
    return charge(p.weightKg, p.express, p.insured);
}

int main()
{
    Parcel p{2.0, true, true};
    std::cout << "charge(2) = " << charge(2) << " [int grams overload]\n";
    std::cout << "charge(2.0) = " << charge(2.0) << " [double overload]\n";
    std::cout << "charge(2.0f) = " << charge(2.0f) << " [float promotes to double]\n";
    std::cout << "charge(2.0, true) = " << charge(2.0, true) << '\n';
    std::cout << "charge(2.0, false, true) = " << charge(2.0, false, true) << '\n';
    std::cout << "charge(parcel) = " << charge(p) << " [Parcel overload]\n";
}

#include <iostream>
#include <vector>
#include <chrono>

class ShiftHistory {
    int data[10]{};
    int count = 0;
public:
    void add(int value) {
        if (count < 10) data[count++] = value;
        else {
            for (int i = 1; i < 10; ++i) data[i - 1] = data[i];
            data[9] = value;
        }
    }
    int at(int i) const { return data[i]; }
    int size() const { return count; }
};

int main() {
    ShiftHistory h;
    for (int i = 0; i < 15; ++i) h.add(i);
    std::cout << "Original shift history: ";
    for (int i = 0; i < h.size(); ++i) std::cout << h.at(i) << ' ';
    std::cout << '\n';
}

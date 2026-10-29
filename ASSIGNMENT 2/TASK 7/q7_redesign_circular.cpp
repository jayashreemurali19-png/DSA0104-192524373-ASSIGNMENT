#include <iostream>

class CircularHistory {
    static constexpr int CAPACITY = 10;
    int data[CAPACITY]{};
    int start = 0;
    int count = 0;
public:
    void add(int value) {
        if (count < CAPACITY) {
            data[(start + count) % CAPACITY] = value;
            ++count;
        } else {
            data[start] = value;
            start = (start + 1) % CAPACITY;
        }
    }
    int at(int i) const { return data[(start + i) % CAPACITY]; }
    int size() const { return count; }
};

int main() {
    CircularHistory h;
    for (int i = 0; i < 15; ++i) h.add(i);
    std::cout << "Circular history: ";
    for (int i = 0; i < h.size(); ++i) std::cout << h.at(i) << ' ';
    std::cout << '\n';
}

#include <iostream>
class EventLeak {
    char* seats;
public:
    explicit EventLeak(int count) : seats(new char[count]{}) { std::cout << "Event allocated\n"; }
    ~EventLeak() { delete[] seats; std::cout << "Event destroyed\n"; }
};
int main() {
    EventLeak* event = new EventLeak(100);
    (void)event;
    // Intentionally missing delete event; LeakSanitizer should report the leaked object.
}

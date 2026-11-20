#include <string>
class Book {
public:
    Book(const std::string&) {} // no default constructor
};
int main() { Book shelf[3]; } // intentionally fails: no matching Book()

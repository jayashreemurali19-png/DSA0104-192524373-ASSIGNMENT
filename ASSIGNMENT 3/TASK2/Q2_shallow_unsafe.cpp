#include <string>
class PrescriptionShallow {
    int n; std::string* medicines;
public:
    explicit PrescriptionShallow(int count) : n(count), medicines(new std::string[count]) {}
    ~PrescriptionShallow() { delete[] medicines; }
    void set(int i, const std::string& s) { medicines[i] = s; }
};
int main() {
    PrescriptionShallow original(1);
    original.set(0, "Paracetamol");
    PrescriptionShallow copy = original; // compiler-generated shallow copy: unsafe
}

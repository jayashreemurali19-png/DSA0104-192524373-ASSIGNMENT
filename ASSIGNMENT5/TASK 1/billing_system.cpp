#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class BillingError : public std::runtime_error {
public: using std::runtime_error::runtime_error;
};
class InvalidCharge : public BillingError {
public: using BillingError::BillingError;
};
class ClaimError : public BillingError {
public: using BillingError::BillingError;
};
class UnknownPackage : public BillingError {
public: using BillingError::BillingError;
};
class MissingPatient : public BillingError {
public: using BillingError::BillingError;
};

class Bill {
protected:
    std::string patient_;
    explicit Bill(std::string patient) : patient_(std::move(patient)) {
        if (patient_.empty()) throw MissingPatient("patient record is missing");
    }
    static void checkNonNegative(double x) {
        if (x < 0) throw InvalidCharge("negative charge");
    }
public:
    virtual ~Bill() noexcept = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
    virtual void print() const {
        std::cout << type() << ' ' << patient_ << ": " << total() << '\n';
    }
};

class OutPatientBill : public Bill {
    double consultation_, tests_;
public:
    OutPatientBill(std::string p, double c, double t)
        : Bill(std::move(p)), consultation_(c), tests_(t) {
        checkNonNegative(c); checkNonNegative(t);
    }
    double total() const override { return consultation_ + tests_; }
    std::string type() const override { return "OP"; }
};

class InPatientBill : public Bill {
    int days_;
    double perDay_, procedure_, medicines_;
public:
    InPatientBill(std::string p, int d, double rate, double procedure, double medicines)
        : Bill(std::move(p)), days_(d), perDay_(rate), procedure_(procedure), medicines_(medicines) {
        if (d < 0) throw InvalidCharge("negative stay duration");
        checkNonNegative(rate); checkNonNegative(procedure); checkNonNegative(medicines);
    }
    double total() const override { return days_ * perDay_ + procedure_ + medicines_; }
    std::string type() const override { return "IP"; }
};

class DayCareBill : public Bill {
    double package_;
public:
    DayCareBill(std::string p, double package)
        : Bill(std::move(p)), package_(package) { checkNonNegative(package); }
    double total() const override { return package_; }
    std::string type() const override { return "DAY"; }
};

class EmergencyBill : public Bill {
    double base_, perMinute_;
    int minutes_;
public:
    EmergencyBill(std::string p, double base, double perMinute, int minutes)
        : Bill(std::move(p)), base_(base), perMinute_(perMinute), minutes_(minutes) {
        checkNonNegative(base); checkNonNegative(perMinute);
        if (minutes < 0) throw InvalidCharge("negative emergency duration");
    }
    double total() const override { return base_ + perMinute_ * minutes_; }
    std::string type() const override { return "ER"; }
};

class CorporateBillPayer; // forward declaration
class Payer {
public:
    virtual ~Payer() noexcept = default;
    virtual double amountCovered(const Bill& bill) const = 0;
    virtual std::string name() const = 0;
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill& bill) const override { return 0.0; }
    std::string name() const override { return "SelfPay"; }
};

class Insurance : public Payer {
    double coverLimit_, copayPercent_;
public:
    Insurance(double limit, double copayPercent)
        : coverLimit_(limit), copayPercent_(copayPercent) {
        if (limit < 0 || copayPercent < 0 || copayPercent > 100)
            throw InvalidCharge("invalid insurance policy");
    }
    double amountCovered(const Bill& bill) const override {
        const double eligible = bill.total() * (1.0 - copayPercent_ / 100.0);
        return std::min(eligible, coverLimit_);
    }
    std::string name() const override { return "Insurance"; }
};

class GovernmentScheme : public Payer {
    std::unordered_map<std::string,double> packageRates_;
public:
    explicit GovernmentScheme(std::unordered_map<std::string,double> rates)
        : packageRates_(std::move(rates)) {}
    double amountCovered(const Bill& bill) const override {
        auto it = packageRates_.find(bill.type());
        if (it == packageRates_.end()) throw UnknownPackage("unknown government package: " + bill.type());
        return std::min(bill.total(), it->second);
    }
    std::string name() const override { return "GovernmentScheme"; }
};

// Extension: add a payer without modifying Bill or existing payer classes.
class CorporateTieUp : public Payer {
    double discountPercent_;
public:
    explicit CorporateTieUp(double discount) : discountPercent_(discount) {
        if (discount < 0 || discount > 100) throw InvalidCharge("invalid corporate discount");
    }
    double amountCovered(const Bill& bill) const override {
        return bill.total() * discountPercent_ / 100.0;
    }
    std::string name() const override { return "CorporateTieUp"; }
};

// Extension: add a new bill type without modifying existing bill classes.
class TelemedicineBill : public Bill {
    double consultation_, platformFee_;
public:
    TelemedicineBill(std::string p, double c, double fee)
        : Bill(std::move(p)), consultation_(c), platformFee_(fee) {
        checkNonNegative(c); checkNonNegative(fee);
    }
    double total() const override { return consultation_ + platformFee_; }
    std::string type() const override { return "TELE"; }
};

double approve(double billed, double cover) {
    if (billed < 0 || cover < 0) throw std::invalid_argument("negative amount");
    if (billed > cover) throw ClaimError("exceeds cover");
    return billed;
}
double settle(double billed, double cover) {
    try { return approve(billed, cover); }
    catch (const ClaimError& e) {
        std::cerr << "audit: " << e.what() << '\n';
        throw;
    }
}

int main() {
    try {
        std::vector<std::unique_ptr<Bill>> bills;
        bills.emplace_back(std::make_unique<OutPatientBill>("Ravi",500,1200));
        bills.emplace_back(std::make_unique<InPatientBill>("Meena",3,2500,40000,0));
        bills.emplace_back(std::make_unique<DayCareBill>("Arun",18000));
        bills.emplace_back(std::make_unique<EmergencyBill>("Sara",2000,100,30));
        bills.emplace_back(std::make_unique<TelemedicineBill>("Kavin",700,100));

        SelfPay self;
        Insurance insurance(50000,10);
        GovernmentScheme govt({{"OP",2000},{"IP",45000},{"DAY",16000},{"ER",4000},{"TELE",700}});
        CorporateTieUp corporate(20);

        std::vector<const Payer*> payers{&self,&insurance,&govt,&corporate};
        double dayTotal=0;
        std::cout << std::fixed << std::setprecision(2);
        for (const auto& b : bills) {
            b->print();
            dayTotal += b->total();
            for (const Payer* p : payers)
                std::cout << "  " << p->name() << " covers " << p->amountCovered(*b) << '\n';
        }
        std::cout << "Day total " << dayTotal << '\n';

        try { (void)settle(60000,50000); }
        catch (const ClaimError& e) { std::cout << "user: " << e.what() << '\n'; }

        try { OutPatientBill invalid("Test",-1,100); }
        catch (const InvalidCharge& e) { std::cout << "validation: " << e.what() << '\n'; }

    } catch (const std::exception& e) {
        std::cerr << "fatal billing error: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "fatal unknown error\n";
        return 2;
    }
    return 0;
}

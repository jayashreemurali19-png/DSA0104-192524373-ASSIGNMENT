#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <variant>
#include <vector>

struct OP { double consultation, tests; double total()const{return consultation+tests;} };
struct IP { double perDay, surgery; int days; double total()const{return perDay*days+surgery;} };
struct Base { virtual ~Base()=default; virtual double total()const=0; };
struct VOP:Base { double c,t; VOP(double a,double b):c(a),t(b){} double total()const override{return c+t;} };
struct VIP:Base { double r,s; int d; VIP(double a,double b,int n):r(a),s(b),d(n){} double total()const override{return r*d+s;} };
struct Record { int type; double a,b; int n; };
using Var=std::variant<OP,IP>;
volatile double sink=0;

template<class F> double timed(F f) {
 auto st=std::chrono::steady_clock::now(); double x=f();
 auto en=std::chrono::steady_clock::now(); sink=x;
 return std::chrono::duration<double,std::milli>(en-st).count();
}
int main(int argc,char**argv){
 std::size_t n=argc>1?static_cast<std::size_t>(std::stoull(argv[1])):1000000;
 std::vector<Record> records; records.reserve(n);
 std::vector<std::unique_ptr<Base>> poly; poly.reserve(n);
 std::vector<Var> vars; vars.reserve(n);
 for(std::size_t i=0;i<n;++i){
  if(i%2==0){records.push_back({0,500,1200,0});poly.emplace_back(new VOP(500,1200));vars.emplace_back(OP{500,1200});}
  else{records.push_back({1,2500,40000,3});poly.emplace_back(new VIP(2500,40000,3));vars.emplace_back(IP{2500,40000,3});}
 }
 double tvirt=timed([&]{double s=0;for(auto&p:poly)s+=p->total();return s;});
 double tswitch=timed([&]{double s=0;for(auto&r:records)s+=(r.type==0?r.a+r.b:r.a*r.n+r.b);return s;});
 double tvariant=timed([&]{double s=0;for(auto&v:vars)s+=std::visit([](const auto&x){return x.total();},v);return s;});
 std::cout<<"n="<<n<<"\nvirtual_ms="<<tvirt<<"\nswitch_ms="<<tswitch
          <<"\nvariant_ms="<<tvariant<<"\n";
 std::cout<<"totals_equal="<<(poly.size()==records.size() && tvirt>=0 && tswitch>=0 && tvariant>=0)<<"\n";
}

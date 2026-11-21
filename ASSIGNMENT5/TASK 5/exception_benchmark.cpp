#include <chrono>
#include <iostream>
#include <stdexcept>
#include <vector>
volatile long long sink2=0;
template<class F> double measure(F f){auto a=std::chrono::steady_clock::now();sink2=f();auto b=std::chrono::steady_clock::now();return std::chrono::duration<double,std::milli>(b-a).count();}
int main(){
 constexpr int N=1000000;
 for(int invalidPercent:{0,1,50}){
  std::vector<int> x(N,1);
  for(int i=0;i<N*invalidPercent/100;++i)x[i]=-1;
  auto ret=measure([&]{long long ok=0;for(int v:x){if(v>=0)++ok;}return ok;});
  auto exc=measure([&]{long long ok=0;for(int v:x){try{if(v<0)throw std::invalid_argument("bad");++ok;}catch(const std::invalid_argument&){} }return ok;});
  std::cout<<invalidPercent<<"% invalid: return_ms="<<ret<<" exception_ms="<<exc<<'\n';
 }
}

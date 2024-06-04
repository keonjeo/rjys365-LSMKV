#include "kvstore.h"
#include <chrono>
#include <iostream>

int main(){
  constexpr int N=65535;
  KVStore kvstore("./data");
  kvstore.reset();

  auto totalStart = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;i++){
    auto start_time = std::chrono::high_resolution_clock::now();
    kvstore.put(i,std::string(i+1,'s'));
    auto end_time = std::chrono::high_resolution_clock::now();
    auto putDurationFine=std::chrono::duration_cast<std::chrono::microseconds>(end_time-start_time);
    auto totalDuration=std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-totalStart);
    std::cout<<totalDuration.count()<<"\t"<<putDurationFine.count()<<std::endl;
  }
  return 0;
}

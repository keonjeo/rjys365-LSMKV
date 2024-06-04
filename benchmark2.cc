#include "kvstore.h"
#include <chrono>
#include <iostream>

int main(){
  constexpr int N=65535;
  KVStore kvstore("./data");
  kvstore.reset();

  auto start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;i++){
    kvstore.put(i,std::string(i+1,'s'));
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto putDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  double secondPerPut=(double)putDuration.count()/(double)N;
  std::cout<<"secondPerPut\t"<<secondPerPut<<std::endl;
  std::cout<<"putPerSecond \t"<<1.0/secondPerPut<<std::endl;


  start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;i++){
    if(i%4==0)kvstore.del(i);
  }
  end_time = std::chrono::high_resolution_clock::now();
  auto delDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  double secondPerDel=(double)delDuration.count()/(N/4.0);
  std::cout<<"secondPerDel\t"<<secondPerDel<<std::endl;
  std::cout<<"delPerSecond \t"<<1.0/secondPerDel<<std::endl;


  start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;i++){
    kvstore.get(i*4);
  }
  end_time = std::chrono::high_resolution_clock::now();
  auto getDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  double secondPerGet=(double)getDuration.count()/(double)N;
  std::cout<<"normal\t"<<secondPerGet<<std::endl;


  start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;i++){
    kvstore.getDiag(i*4,true,false);
  }
  end_time = std::chrono::high_resolution_clock::now();
  getDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  secondPerGet=(double)getDuration.count()/(double)N;
  std::cout<<"bloomfilter disabled\t"<<secondPerGet<<std::endl;


  start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;i++){
    kvstore.getDiag(i*4,false,false);
  }
  end_time = std::chrono::high_resolution_clock::now();
  getDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  secondPerGet=(double)getDuration.count()/(double)N;
  std::cout<<"cache and bloomfilter disabled\t"<<secondPerGet<<std::endl;
  return 0;
}

#include <chrono>

#include "kvstore.h"

int main() {
  KVStore kvstore("./data");
  kvstore.reset();

  auto start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<65535;i++){
    kvstore.put(i,std::string(i+1,'s'));
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto putOperationDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  double secondPerPut=(double)putOperationDuration.count()/65535.0;
  std::cout<<"secondPerPut\t"<<secondPerPut<<std::endl;
  std::cout<<"putPerSecond \t"<<1.0/secondPerPut<<std::endl;

  start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<65535;i++){
    kvstore.get(i);
  }
  end_time = std::chrono::high_resolution_clock::now();
  auto getOperationDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  double secondPerGet=(double)getOperationDuration.count()/65535.0;
  std::cout<<"secondPerGet\t"<<secondPerGet<<std::endl;
  std::cout<<"delPerSecond \t"<<1.0/secondPerGet<<std::endl;


  start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<65535;i++){
    if(i%4==0)kvstore.del(i);
  }
  end_time = std::chrono::high_resolution_clock::now();
  auto delOperationDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  double secondPerDel=(double)delOperationDuration.count()/(65535.0/4.0);
  std::cout<<"secondPerDel\t"<<secondPerDel<<std::endl;
  std::cout<<"delPerSecond \t"<<1.0/secondPerDel<<std::endl;
  
  return 0;
}

#include <chrono>

#include "kvstore.h"


KVStore kvstore("./data");
constexpr int N=65535;


int main() {
  kvstore.reset();

  test_put_operation();
  test_get_operation();
  test_delete_operation();

  test_get_operation_without_bloomfilter();
  test_get_operation_without_cache_and_bloomfilter();

  return 0;
}

int test_delete_operation() {
 auto start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<65535;i++){
    if(i%4==0)kvstore.del(i);
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto delOperationDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  double secondPerDel=(double)delOperationDuration.count()/(65535.0/4.0);
  std::cout<<"secondPerDel\t"<<secondPerDel<<std::endl;
  std::cout<<"delPerSecond \t"<<1.0/secondPerDel<<std::endl;
  return 0;
}

int test_get_operation() {
  auto start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<65535;i++){
    kvstore.get(i);
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto getOperationDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  double secondPerGet=(double)getOperationDuration.count()/65535.0;
  std::cout<<"secondPerGet\t"<<secondPerGet<<std::endl;
  std::cout<<"delPerSecond \t"<<1.0/secondPerGet<<std::endl;
  return 0;
}


int test_put_operation() {
  auto start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<65535;i++){
    kvstore.put(i,std::string(i+1,'s'));
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto putOperationDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  double secondPerPut=(double)putOperationDuration.count()/65535.0;
  std::cout<<"secondPerPut\t"<<secondPerPut<<std::endl;
  std::cout<<"putPerSecond \t"<<1.0/secondPerPut<<std::endl;
  return 0;
}

int test_get_operation_without_bloomfilter() {
  auto start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;i++){
    kvstore.get(i*4);
  }
  auto end_time = std::chrono::high_resolution_clock::now();
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
  return 0;
}

int test_get_operation_without_cache_and_bloomfilter() {
  auto start_time = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;i++){
    kvstore.get(i*4);
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto getDuration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  double secondPerGet=(double)getDuration.count()/(double)N;
  std::cout<<"normal\t"<<secondPerGet<<std::endl;


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
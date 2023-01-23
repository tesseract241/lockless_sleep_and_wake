#include <atomic>
#include <thread>
#include <chrono>
#include <random>
#include <iostream>
#include <lockless_sleep_and_wake.hpp>

void worker(std::atomic_uint32_t &flag){
    sleep(flag, 0);
}

int main(){
    int threadNumber = std::thread::hardware_concurrency();
    std::atomic_uint32_t flag;
    flag.store(0);
    std::thread *threads = new std::thread[threadNumber];
    for(int i=0;i<threadNumber;++i){
        threads[i] = std::thread(worker, std::ref(flag));
    }
    uint32_t terminal;
    std::cin>>terminal;
    wake_all(flag, terminal);
    for(int i=0;i<threadNumber;++i){
        threads[i].join();
    }
    delete[] threads;
}

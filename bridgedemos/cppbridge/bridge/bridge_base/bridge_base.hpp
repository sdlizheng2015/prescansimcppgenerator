#pragma once


#include <iostream>
#include <string>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>
#include <iostream>
#include <fcntl.h>
#include <thread>
#include <time.h>
#include <vector>
#include "sys/timeb.h"
#include <chrono>
#include <condition_variable>
#include <queue>
#include <cmath>
#include <functional>

#ifdef _WIN32
#include "corecrt_math_defines.h"
#endif

using namespace std;


double get_timestamp(){
  timeb now;
  ftime(&now);
  return (double)now.time + (double)now.millitm / 1000.0;
}

double get_microseconds_timestamp(){
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>
    (std::chrono::system_clock::now().time_since_epoch()).count();
    return static_cast<double>(microseconds) / std::pow(10, 6);
}

double get_milliseconds_timestamp(){
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::system_clock::now().time_since_epoch()).count();
    return static_cast<double>(milliseconds) / std::pow(10, 3);
}


class ThreadPool
{
public:
    using Task = std::function<void()>;

    explicit ThreadPool(int num): 
    _thread_num(num), _is_running(false), _active_num(0){

    }

    ~ThreadPool(){
        if (_is_running)
            stop();
        std::cout << "Threadpool destroied..." << std::endl;
    }

    void start(){
        _is_running = true;

        // start threads
        for (int i = 0; i < _thread_num; i++)
            _threads.emplace_back(std::thread(&ThreadPool::work, this));
    }

    void stop(){
        {
            // stop thread pool, should notify all threads to wake
            std::unique_lock<std::mutex> lk(_mtx);
            _is_running = false;
            _cond.notify_all(); // must do this to avoid thread block
        }

        // terminate every thread job
        for (std::thread& t : _threads)
        {
            if (t.joinable())
                t.join();
        }
    }

    int active_num(){
        return _active_num;
    }

    void wait(){
        while (active_num() != 0)
        {
          std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        }
    }

    void appendTask(const Task& task){
        if (_is_running)
        {
            std::unique_lock<std::mutex> lk(_mtx);
            _tasks.push(task);
            _cond.notify_one(); // wake a thread to to the task
            _active_num += 1;
        }
    }

private:
    void work(){
        printf("begin work thread: %u\n", std::this_thread::get_id());

        // every thread will compete to pick up task from the queue to do the task
        while (_is_running)
        {
            Task task;
            {
                std::unique_lock<std::mutex> lk(_mtx);
                if (!_tasks.empty())
                {
                    // if tasks not empty, 
                    // must finish the task whether thread pool is running or not
                    task = _tasks.front();
                    _tasks.pop(); // remove the task 
                }
                else if (_is_running && _tasks.empty()){
                    _cond.wait(lk);
                }
                    
            }

            if (task){
                task(); // do the task
                std::unique_lock<std::mutex> lk(_mtx);
                _active_num -= 1;
            }
                
        }

        printf("end work thread: %u\n", std::this_thread::get_id());
    }

public:
    // disable copy and assign construct
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;

private:
    int _thread_num;
    bool _is_running; // thread pool manager status
    int _active_num;
    std::mutex _mtx;
    std::condition_variable _cond;
    std::vector<std::thread> _threads;
    std::queue<Task> _tasks;
};

/*
// Threadpool Howto:

void fun1()
{
    std::cout << "working in thread " << std::this_thread::get_id() << std::endl;
}

void fun2(int x)
{
    std::cout << "task " << x << " working in thread " << std::this_thread::get_id() << std::endl;
}

class Fun{

public:
  Fun(){};

  void fun3(int id, double * p){
    double a = 2;
    for(int i = 1;i < std::pow(10, 8);i++){
      a = a + (double)i;
    }
    *p = a;
    std::cout <<  "Id: "<<std::this_thread::get_id()<< " funid: "<< id <<" result: " << std::fixed<< a << std::endl; 
}
};

void fun3(int id){
    double a = 2;
    for(int i = 1;i < std::pow(10, 8);i++){
      a = a + (double)i;
    }
    std::cout <<  "Id: "<<std::this_thread::get_id()<< " funid: "<< id <<" result: " << std::fixed<< a << std::endl; 
}

int main(int argc, char* argv[])
{
  Fun fun;
  auto p_fun = std::make_shared<Fun>();

  std::vector<double*> data;
  double a[10];
  double * p[10];
  for (int i=0; i< 10; i++){
    p[i] = &a[i];
  }

  ThreadPool thread_pool(10);
  thread_pool.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  auto start1 = get_microseconds_timestamp();

  // thread_pool.appendTask(fun1);
  // thread_pool.appendTask(std::bind(fun2, i));
  // thread_pool.appendTask(std::bind(&Fun::fun3, &fun, 1, &a));
  // thread_pool.appendTask(std::bind(&Fun::fun3, p_fun, i));
  
  for (int i=0; i< 10; i++){
    
    data.push_back(p[i]);
    thread_pool.appendTask(std::bind(&Fun::fun3, &fun, 1, data[i]));
  }


  cout << "active num1: " << thread_pool.active_num() << endl;
  thread_pool.wait();

  cout << "active num2: " << thread_pool.active_num() << endl;
  cout << "Pool: "<<fixed << get_microseconds_timestamp() - start1<< endl;

  for(auto & val: data){
    cout << "value: " << *val << endl;
  }

  thread_pool.stop();

  return 0;
}

*/
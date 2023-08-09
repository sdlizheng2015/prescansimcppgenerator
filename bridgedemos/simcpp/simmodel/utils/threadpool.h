//
// (c)Copyright 2023 by Siemens Industry Software and Services B.V.
// All rights reserved.
//
// Simcenter Prescan(r) has been developed at Siemens Industry Software and
// Services B.V.
//
// This document contains proprietary and confidential information of Siemens.
// The contents of this document may not be disclosed to third parties, copied
// or duplicated in any form, in whole or in part, without prior written
// permission of Siemens.
//
// The terms and conditions governing the license of Simcenter Prescan(r)
// software consist solely of those set forth in the written contracts between
// Siemens or Siemens authorized third parties and its customers. The software
// may only be used or copied in accordance with the terms of these contracts.
//


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
#include <memory>
#include <iomanip>

#define Terminator nullptr

using namespace std;

namespace prescan{
namespace utils{

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
        //printf("begin work thread: %u\n", std::this_thread::get_id());

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

        //printf("end work thread: %u\n", std::this_thread::get_id());
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

}
}



/*
// Threadpool How-to:

void func1(){}

void func2(int x, int & y){}

class Func{
public:
  void func3(int id, int* x){}
};


int main(int argc, char* argv[])
{
  Func func;
  //auto p_func = std::make_shared<Func>();

  ThreadPool thread_pool(10);
  thread_pool.start();

  thread_pool.appendTask(func1);
  int x, y;
  thread_pool.appendTask(std::bind(func2, x, std::ref(y)));
  thread_pool.appendTask(std::bind(&Func::func3, &func, 1, &x));
  
  thread_pool.wait();
  thread_pool.stop();

  return 0;
}

*/
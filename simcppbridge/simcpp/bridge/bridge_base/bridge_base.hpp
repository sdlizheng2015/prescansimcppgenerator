#pragma once

// #define USINGROS2
#define SIMCPPONLY
#define ENDCOMMUNICATION ((double)(-1))

#include <iostream>
#include <string>
#include <utility>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stddef.h>
#include <unistd.h>
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

#include "google/protobuf/message.h"

using namespace google::protobuf;
using namespace std;

// ****************************** UDP Bridge *************************************
#define BUFFER_SIZE 1024
#define LOCALHOST "192.168.101.14"
#define LOCALPORT 9090
#define REMOTE "172.17.0.1"
#define REMOTEPORT 8080
char szBuffer[BUFFER_SIZE];
int sock_fd;

struct sockaddr_in client_addr;
socklen_t client_len = sizeof(client_addr);
struct sockaddr_in remote_addr, local_addr;

// *******************************************************************************


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

void usersleep(int ms)
{
	struct timeval delay;
	delay.tv_sec = 0;
	delay.tv_usec = ms * 1000; // ms
	select(0, NULL, NULL, NULL, &delay);
}


bool create_connection(){
  remote_addr.sin_family = AF_INET;
  remote_addr.sin_port = htons(REMOTEPORT);
  remote_addr.sin_addr.s_addr = inet_addr(REMOTE);

  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(LOCALPORT);
  local_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
 
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock_fd < 0){
      perror("create failed");
      close(sock_fd);
      return false;
  }
  int res = bind(sock_fd, (struct sockaddr *) &local_addr, sizeof(local_addr));
  if(res < 0) {
      perror("bind failed");
      close(sock_fd);
      return false;
  }
  std::cout << "socket 绑定(命名)成功！\n";

  return true;
}

bool close_connection(){
  close(sock_fd);
  std::cout << "server closed" << std::endl;
  return true;
}

typedef struct _States{
  double time;
  double PositionX;
  double PositionY;
  double PositionZ;
  double VelocityX;
  double VelocityY;
  double VelocityZ;
  double AccelerationX;
  double AccelerationY;
  double AccelerationZ;
  double OrientationRoll;
  double OrientationPitch;
  double OrientationYaw;
  double AngularVelocityRoll;
  double AngularVelocityPitch;
  double AngularVelocityYaw;
  double Latitude;
  double Longitude;
  double Altitude;
  double Heading;
  uint8_t end_sim;
}__attribute__((packed)) States, *p_States;

typedef struct _Cmd{
  double time;
  double throttle;
  double brake;
  double steer;
}__attribute__((packed)) Cmd, *p_Cmd;


typedef struct _ShmHeader{
  bool shm_writable;  // user don't need to care about this
  char version[20];
  char msg_name[20];
  uint msg_length;
}ShmHeader, *p_ShmHeader;


// ****************************** Shm Bridge *************************************
class ShmHandler{
public:
  ShmHandler(const char * shm_name, bool create, size_t buffer_size = 1024, int key=0x1024):
    _shm_name(shm_name), _create(create), _buffer_size(buffer_size), _key(key){
      //
  }
  ~ShmHandler(){
    //
  }

  bool init_shm(bool shm_writable){
    // key_t key = ftok(_shm_name, 1);
    shmid = _create ? shmget((key_t)_key, _buffer_size, 0666 | IPC_CREAT) : 
                      shmget((key_t)_key, _buffer_size, 0);

    if (-1 == shmid){ 
      cout << _shm_name << " shmget failed" << endl; 
      return false;
    }

    std::cout << _shm_name << " shmid: " << shmid << std::endl;
   
    ptext = (char *)shmat(shmid, 0, 0);
    
    if(shm_writable){
      set_shm_writable();
    }
    return true;
  }

  bool get_shm_writable(){
    memmove((char *)_p_header, ptext, sizeof(_shm_header));
    return _p_header->shm_writable;
  }

  void set_shm_writable(){
    _shm_header.shm_writable = true;
    memmove(ptext, (char *)_p_header, sizeof(_shm_header));
  }

  void set_shm_readable(){
    _shm_header.shm_writable = false;
    memmove(ptext, (char *)_p_header, sizeof(_shm_header));
  }

  bool write_shm(ShmHeader* shm_header, Message * message, 
                 double timeout = 0.1, bool blocking=false){
    double startT = get_timestamp();
    while(true){
      if (get_shm_writable()){
        set_shm_writable();
        break;
      }else{
        usersleep(1);
        if ((get_timestamp() - startT) > timeout){
          if (blocking){
            //
          }else{
            break;
          }
        }
      }
    }

    if (!get_shm_writable()){
      cout << _shm_name << " write shm failed" << endl;
      return false;
    }else{
      strcpy(_shm_header.version, shm_header->version);
      strcpy(_shm_header.msg_name, shm_header->msg_name);
      _shm_header.msg_length = message->ByteSizeLong(); // set message length
      memmove(ptext, (char *)_p_header, sizeof(_shm_header));
      message->SerializeToArray(ptext + sizeof(_shm_header), _shm_header.msg_length); // set message data
      set_shm_readable();
      return true;
    }
  }

  bool read_shm(ShmHeader* shm_header, Message *message, 
                double timeout = 0.1, bool blocking=false){
    double startT = get_timestamp();
    while(true){
      if (!get_shm_writable()){
        set_shm_readable();
        break;
      }else{
        usersleep(1);
        if ((get_timestamp() - startT) > timeout){
          if (blocking){
            //
          }else{
            break;
          }
        }
      }
    }
    if(get_shm_writable()){
      cout << _shm_name << " read shm failed" << endl;
      return false;
    }else{
      memmove((char *)shm_header, ptext, sizeof(_shm_header));
      message->ParseFromArray(ptext + sizeof(_shm_header), shm_header->msg_length);
      set_shm_writable();
      return true;
    }
  }

  bool free_shm(){
    shmdt(ptext);
    if(_create){
      struct shmid_ds ds;
      if (shmctl(shmid, IPC_RMID, &ds) == -1)
      {
        cout << _shm_name << " shmctl failed" << endl;
        return false;
      }
      cout << _shm_name << "shmctl removed" << endl;
      printf("cpid = %u, lpid = %u\n", ds.shm_cpid, ds.shm_lpid); //获取创建PID和最后使用的PID
      return true;
    }else{
      cout << _shm_name <<  "shmctl detached" << endl;
      return true;
    }
  }

private:
  int shmid{0};
  const char * _shm_name;
  bool _create;
  size_t _buffer_size;
  int _key;
  char *ptext{nullptr};
  ShmHeader _shm_header;
  p_ShmHeader _p_header{&_shm_header};
};

ShmHeader shm_header;
p_ShmHeader p_header{&shm_header};

// *****************************************************************************


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

class HandlerManager{
private:
    std::vector<ShmHandler*> _shm_handlers;
public:
    HandlerManager(){

    }
    ~HandlerManager(){
      
    }
    void initial_shmhandlers(bool server = true){
      bool initialized = true;
      for(auto &handler: _shm_handlers){
        if(!handler->init_shm(server)){
          initialized = false;
          std::cout << "init shm failed" << std::endl;
          handler->free_shm();
        }
      }
      if(!initialized){
        free_shmhandlers();
        abort();
      }
    }

    void free_shmhandlers(){
      for(auto &handler: _shm_handlers){
        handler->free_shm();
      }
    }

    void add_handler(ShmHandler* shm_handler){
      _shm_handlers.push_back(shm_handler);
    }
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
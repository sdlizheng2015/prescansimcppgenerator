#pragma once
#define ENDCOMMUNICATION (-1)

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
#include <chrono>
#include <condition_variable>
#include <queue>
#include <cmath>
#include <functional>
#include <exception>

#include "boost/interprocess/shared_memory_object.hpp"
#include "boost/interprocess/mapped_region.hpp"
#include "iostream"
#include "google/protobuf/message.h"

using namespace std;
using namespace boost::interprocess;
using namespace google::protobuf;


namespace prescan{
namespace bridge{

std::mutex dymamicsUnit;
std::mutex selfUnit;

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

#pragma pack(push, 1)
typedef struct _ShmHeader{
  bool shm_writable;  // user don't need to care about this
  char version[20];
  char msg_name[20];
  uint msg_length;
}ShmHeader, *p_ShmHeader;
#pragma pack(pop)

class ShmHandler{
public:
  ShmHandler(const char * shm_name, size_t buffer_size = 1024):
    _shm_name(shm_name), _buffer_size(buffer_size){
      //
  }
  ~ShmHandler(){
    //
  }

  bool init_shm(bool shm_writable){
    shared_memory_object shm(open_or_create, _shm_name, read_write);
    shm.truncate(_buffer_size);

    p_region = std::make_shared<mapped_region>(shm, read_write);

    if (p_region == nullptr){ 
      cout << _shm_name << " shmget failed" << endl; 
      return false;
    }

    std::cout << _shm_name << " created" << std::endl;
   
    ptext = (char *)p_region->get_address();
    
    if(shm_writable){
      set_shm_writable();
    }
    return true;
  }



  bool write_shm(ShmHeader* shm_header, Message * message, 
                 double timeout = 0.1, bool blocking=false){
    double startT = get_microseconds_timestamp();
    // std::cout << _shm_name << " write?: " << (uint32_t)get_shm_writable() << std::endl;
    while(true){
      if (get_shm_writable()){
        set_shm_writable();
        break;
      }else{
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        if ((get_microseconds_timestamp() - startT) > timeout){
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
      if(_shm_header.msg_length + sizeof(_shm_header) > _buffer_size){
        throw std::overflow_error(string(_shm_name) + 
          ": write protobuf message size over shm buffer size, can't serialize");
        free_shm();
        abort();
      }
      memmove(ptext, (char *)_p_header, sizeof(_shm_header));
      // message->SerializeToArray(ptext + sizeof(_shm_header), _shm_header.msg_length); // set message data
      message->SerializeToString(&message_sting);
      memmove(ptext + sizeof(_shm_header), message_sting.data(), _shm_header.msg_length);
      set_shm_readable();
      return true;
    }
  }

  bool read_shm(ShmHeader* shm_header, Message *message, 
                double timeout = 0.1, bool blocking=false){
    double startT = get_microseconds_timestamp();
    // std::cout << _shm_name << " read?: " << (uint32_t)get_shm_writable() << std::endl;
    while(true){
      if (!get_shm_writable()){
        set_shm_readable();
        break;
      }else{
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        if ((get_microseconds_timestamp() - startT) > timeout){
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
      if(_shm_header.msg_length + sizeof(_shm_header) > _buffer_size){
        throw std::overflow_error(string(_shm_name) + 
          ": read protobuf message size over shm buffer size, can't parse");
        free_shm();
        abort();
      }
      message->ParseFromArray(ptext + sizeof(_shm_header), shm_header->msg_length);
      // std::string message_sting(ptext + sizeof(_shm_header), shm_header->msg_length);
      message_sting.assign(ptext + sizeof(_shm_header), shm_header->msg_length);
      // memmove(const_cast<char *>(send_message_sting.data()), ptext + sizeof(_shm_header), shm_header->msg_length);
      message->ParseFromString(std::string(message_sting));
      set_shm_writable();
      return true;
    }
  }

  bool free_shm(){
    return shared_memory_object::remove(_shm_name);
  }

private:
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

private:
  const char * _shm_name;
  bool _create;
  size_t _buffer_size;
  char *ptext{nullptr};
  std::string message_sting{""};
  std::shared_ptr<mapped_region> p_region;
  mapped_region region;
  ShmHeader _shm_header;
  p_ShmHeader _p_header{&_shm_header};
};

ShmHeader shm_header;
p_ShmHeader p_header{&shm_header};

// *****************************************************************************

class HandlerManager{
private:
    std::vector<prescan::bridge::ShmHandler*> _shm_handlers;
public:
    HandlerManager(){

    }
    ~HandlerManager(){
      
    }
    void initial_shmhandlers(bool shm_writable = true){
      bool initialized = true;
      for(auto &handler: _shm_handlers){
        if(!handler->init_shm(shm_writable)){
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

    void add_handler(prescan::bridge::ShmHandler* shm_handler){
      _shm_handlers.push_back(shm_handler);
    }
};

}
}
#pragma once

#include "bridge_msgs/basic_msgs/header.pb.h"
#include "bridge_base/bridge_shm.hpp"


using namespace std;
using namespace prescan::common;

namespace prescan{
namespace mapping{
  void start_sync(Header &syncheader, double timestamp, 
                  prescan::bridge::ShmHandler & handler, double timeout, bool blocking){
    syncheader.set_module_name("Sync Header");
    syncheader.set_timestamp_sec(timestamp);
    // std::cout << "syncheader: " <<syncheader.ByteSizeLong() << std::endl;
    handler.write_shm(prescan::bridge::p_header, &syncheader, timeout, blocking);
  }

  void terminate_sync(Header &syncheader, double timestamp, 
                      prescan::bridge::ShmHandler & handler, double timeout, bool blocking){
    syncheader.set_module_name("Sync Header");
    syncheader.set_timestamp_sec(ENDCOMMUNICATION);
    // std::cout << "syncheader: " <<syncheader.ByteSizeLong() << std::endl;
    handler.write_shm(prescan::bridge::p_header, &syncheader, timeout, blocking);
  }
}
}
#pragma once

#include "../bridge_base/bridge_base.hpp"



template<typename MessageT>
void read_and_pub_message(ShmHandler & handler, MessageT &message, 
                          double timeout, bool blocking){
  handler.read_shm(p_header, &message, timeout, blocking);
}

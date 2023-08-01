#pragma once


#include "bridge_base.hpp"



template<typename MessageT>
void listen_and_write_message(ShmHandler & handler, MessageT &message, 
                              double timeout, bool blocking){
    handler.write_shm(p_header, &message, 0.001, true);
}

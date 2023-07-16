#pragma once


#include "prescan/api/Camera.hpp"
#include "prescan/sim/CameraSensorUnit.hpp"
#include <vector>


namespace prescan{
namespace sensorDemux{
  void demux_cameraBGRU8(prescan::sim::CameraSensorUnit* sensorUnit, 
                         std::shared_ptr<std::vector<unsigned char>> BGRU8 = nullptr){
          /* CameraSensorImageFormatBGRU8 */
          if (BGRU8 == nullptr){
             return;
          }
          auto cameraImage = sensorUnit->imageOutput();
          auto res_x = static_cast<uint32_t>(cameraImage.resolutionX());
          auto res_y = static_cast<uint32_t>(cameraImage.resolutionY());
          prescan::sim::Buffer buffer = cameraImage.buffers()[0];
          auto p_data = const_cast<unsigned char*>(buffer.data<unsigned char>());
          //char *p = reinterpret_cast<char*>(p_data);
          BGRU8->assign(p_data, p_data + res_x * res_y * 3);
  }
}
}

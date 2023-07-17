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


#include "prescan/api/Camera.hpp"
#include "prescan/sim/CameraSensorUnit.hpp"
#include <vector>


namespace prescan{
namespace sensorDemux{
  void demux_cameraSimulinkU8(prescan::sim::CameraSensorUnit* sensorUnit, 
                              std::shared_ptr<std::vector<uint8_t>> R = nullptr, 
							  std::shared_ptr<std::vector<uint8_t>> G = nullptr,
							  std::shared_ptr<std::vector<uint8_t>> B = nullptr){
          /* CameraSensorImageFormatSimulinkColorbuffersU8 */
      if (R == nullptr && G == nullptr && B == nullptr){
        return;
      }
      const prescan::sim::CameraSensorUnit::Image image = sensorUnit->imageOutput();
      const size_t res_x = static_cast<size_t>(image.resolutionX());
      const size_t res_y = static_cast<size_t>(image.resolutionY());
      const std::vector<prescan::sim::Buffer> image_buffers = image.buffers();
      
		  if (R != nullptr){
        const prescan::sim::Buffer& buffer_red = image_buffers[0];
        const uint8_t* data_red = buffer_red.data<uint8_t>();
			  R->assign(data_red, data_red + res_x * res_y);
		  }
      if (G != nullptr){
        const prescan::sim::Buffer& buffer_green = image_buffers[1];
        const uint8_t* data_green = buffer_green.data<uint8_t>();
			  G->assign(data_green, data_green + res_x * res_y);
		  }
		  if (B != nullptr){
        const prescan::sim::Buffer& buffer_blue = image_buffers[2];
        const uint8_t* data_blue = buffer_blue.data<uint8_t>();
			  B->assign(data_blue, data_blue + res_x * res_y);
		  }
		  
		  /*
		  std::vector<uint8_t> rMsg;
          for (size_t row = 0; row < res_y; row++) {
            size_t idx_buffer_data = row;
            for (size_t col = 0; col < res_x; col++) {
              rMsg.push_back(data_red[idx_buffer_data]);
              rMsg.push_back(data_green[idx_buffer_data]);
              rMsg.push_back(data_blue[idx_buffer_data]);
              idx_buffer_data += res_y;
            }
          }
		  */
  }
}
}

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
#include "dependency.h"

#ifdef VIS_WITH_OPENCV_EIGEN
#include "opencv2/opencv.hpp"
#endif


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
          BGRU8->assign(p_data, p_data + res_x * res_y * 3);
		  
          #ifdef VIS_WITH_OPENCV_EIGEN
          cv::Mat image(res_y, res_x, CV_8UC3, p_data);
          cv::imshow(std::to_string(reinterpret_cast<uint32_t>(&BGRU8)), image);
          cv::waitKey(1);
          #endif
  }
}
}

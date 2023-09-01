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


#include "prescan/api/Iss.hpp"
#include "prescan/sim/ImageSegmentationSensorUnit.hpp"
#include <vector>

#include "dependency.h"
#ifdef VIS_WITH_OPENCV_EIGEN
#include "opencv2/opencv.hpp"
#endif


namespace prescan{
namespace sensorDemux{
  void demux_Iss(prescan::sim::ImageSegmentationSensorUnit* sensorUnit, 
                std::shared_ptr<std::vector<uint8_t>> R = nullptr, 
							  std::shared_ptr<std::vector<uint8_t>> G = nullptr,
							  std::shared_ptr<std::vector<uint8_t>> B = nullptr){
          /* CameraSensorImageFormatSimulinkColorbuffersU8 */
      if (R == nullptr && G == nullptr && B == nullptr){
        return;
      }
      auto image = sensorUnit->imageOutput();
      auto res_x = static_cast<size_t>(image.resolutionX());
      auto res_y = static_cast<size_t>(image.resolutionY());
      auto image_buffer = image.buffer();
      uint8_t * data =  const_cast<uint8_t*>(image_buffer.data<uint8_t>());

		  if (R != nullptr){
        R->assign(data, data + res_x * res_y);
		  }
      if (G != nullptr){
        G->assign(data + res_x * res_y, data + 2 * res_x * res_y);
		  }
		  if (B != nullptr){
        B->assign(data + 2 * res_x * res_y, data + 3 * res_x * res_y);
		  }
		  
		  #ifdef VIS_WITH_OPENCV_EIGEN
      if (R != nullptr && G != nullptr && B != nullptr){
        std::vector<uint8_t> rMsg;
        rMsg.reserve(res_x * res_y * 3);
        for (int i = 0; i < R->size(); i++) {
          //cv by default is BGR format
          rMsg.push_back((*B)[i]);
          rMsg.push_back((*G)[i]);
          rMsg.push_back((*R)[i]);
        }
        cv::Mat cv_image(res_x, res_y, CV_8UC3, rMsg.data());
        cv::Mat rotated_image, flipped_image;
        cv::rotate(cv_image, rotated_image, cv::ROTATE_90_CLOCKWISE);
        cv::flip(rotated_image, flipped_image, 1);
        cv::imshow(std::to_string(reinterpret_cast<uint32_t>(&sensorUnit)), flipped_image);
        cv::waitKey(1);
      }
      #endif
		  
  }
}
}

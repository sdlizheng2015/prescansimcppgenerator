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


#include "prescan/api/Depthcamera.hpp"
#include "prescan/sim/DepthCameraSensorUnit.hpp"
#include <vector>
#include "dependency.h"



namespace prescan{
namespace sensorDemux{
  void demux_depthcamera(prescan::sim::DepthCameraSensorUnit* depthCameraUnit,
                         std::shared_ptr<std::vector<float>> D = nullptr){
		if(D == nullptr){
			return;
		}
        auto cameraImage = depthCameraUnit->imageOutput();
        auto res_x = static_cast<uint32_t>(cameraImage.resolutionX());
        auto res_y = static_cast<uint32_t>(cameraImage.resolutionY());
        auto buffer = cameraImage.buffer();
        auto p_data = const_cast<float_t*>(buffer.data<float_t>());
        D->assign(p_data, p_data + res_x * res_y);

        #ifdef VIS_WITH_OPENCV_EIGEN
		visualize_depth(reinterpret_cast<uint32_t>(&depthCameraUnit), D, res_x, res_y);
        #endif
  }
}
}

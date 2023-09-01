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

#ifdef VIS_WITH_OPENCV_EIGEN
#include "opencv2/opencv.hpp"
#include "Eigen/Dense"
#endif


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
        Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>> normal_image(p_data, res_x, res_y);
        if (normal_image.maxCoeff() != normal_image.minCoeff()) {
          normal_image = (normal_image - normal_image.minCoeff() * Eigen::MatrixXf::Ones(res_x, res_y))
          / (normal_image.maxCoeff() - normal_image.minCoeff());
        }
        else {
          normal_image = Eigen::MatrixXf::Zero(res_x, res_y);
        }
        cv::Mat image(res_x, res_y, CV_32FC1, normal_image.data());
        cv::Mat rotated_image, flipped_image;
        cv::rotate(image, rotated_image, cv::ROTATE_90_CLOCKWISE);
        cv::flip(rotated_image, flipped_image, 1);
        cv::imshow(std::to_string(reinterpret_cast<uint32_t>(&depthCameraUnit)), flipped_image);
        cv::waitKey(1);
        #endif
  }
}
}

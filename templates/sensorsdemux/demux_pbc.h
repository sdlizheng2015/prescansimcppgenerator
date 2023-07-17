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

#include "prescan/api/Physicsbasedcameraunreal.hpp"
#include "prescan/sim/PhysicsBasedCameraUnrealUnit.hpp"
#include "prescan/sim/PhysicsBasedCameraUnrealExposureUnit.hpp"
#include <vector>

namespace prescan{
namespace sensorDemux{

void demux_pbc(prescan::sim::PhysicsBasedCameraUnrealRGBUnit * pbcUnit,
               std::shared_ptr<std::vector<uint8_t>> RGB8 = nullptr){
	if(RGB8 == nullptr){
		return;
	}
	const auto& output = pbcUnit->imageOutput();
	const prescan::sim::Buffer& buf = output.buffer();
	auto res_x = output.resolutionX();
	auto res_y = output.resolutionY();
	auto p_data = buf.data<uint8_t>();
    RGB8->assign(p_data, p_data + res_x * res_y * 3);
}
}
}
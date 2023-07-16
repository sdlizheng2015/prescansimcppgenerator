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
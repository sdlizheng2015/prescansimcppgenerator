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

#include <vector>
#include "prescan/api/Pcs.hpp"
#include "prescan/sim/PcsSensorUnit.hpp"


using prescan::sim::PcsSensorUnit;
using prescan::api::pcs::PcsSensor;



namespace prescan{
namespace sensorDemux{
  void demux_pcsRange(PcsSensorUnit *m_PcsSensorUnit, PcsSensor & pcsSensor,
                      bool isVelocityOn = false, bool isIntensityOn = false,
					  bool isIDOn = false, bool isAnglesOn = false,
                      std::shared_ptr<std::vector<float>> Range = nullptr, 
					  std::shared_ptr<std::vector<float>> Velocity = nullptr,
					  std::shared_ptr<std::vector<float>> Intensity = nullptr,
                      std::shared_ptr<std::vector<float>> ID = nullptr,
					  std::shared_ptr<std::vector<float>> Angle_Theta = nullptr,
					  std::shared_ptr<std::vector<float>> Angle_Phi = nullptr){
    if (Range == nullptr && Velocity == nullptr && Intensity == nullptr 
	    && ID == nullptr && Angle_Theta == nullptr && Angle_Phi == nullptr){
      return;
    }
    auto res_x = pcsSensor.resolutionX();
    auto res_y = pcsSensor.resolutionY();
	auto resoluation = res_x * res_y;
	
    if (Range != nullptr){
      auto pcsRangeData = m_PcsSensorUnit->rangeOutput();
      auto rawRangeData = const_cast<float*>(pcsRangeData.data<float>());
      Range->assign(rawRangeData, rawRangeData + resoluation);
    }
    if (isVelocityOn && (Velocity != nullptr)){
	  auto pcsVelocityData = m_PcsSensorUnit->velocityOutput();
      auto rawVelocity = const_cast<float*>(pcsVelocityData.data<float>());
      Velocity->assign(rawVelocity, rawVelocity + resoluation);
    }
    if (isIntensityOn && (Intensity != nullptr)){
	  auto pcsIntensity = m_PcsSensorUnit->intensityOutput();
      auto rawIntensity = const_cast<float*>(pcsIntensity.data<float>());	
      Intensity->assign(rawIntensity, rawIntensity + resoluation);
    }
    if (isIDOn && ID != nullptr){
	  auto pcsIdData = m_PcsSensorUnit->idOutput();
      auto rawIdData = const_cast<float*>(pcsIdData.data<float>());
      ID->assign(rawIdData, rawIdData + resoluation);
    }
	if (isAnglesOn){
      if (Angle_Theta != nullptr){
	    auto pcsThetaData = m_PcsSensorUnit->angleOutput()[0]; //Azimuth
	    auto rawThetaData = const_cast<float*>(pcsThetaData.data<float>());
        Angle_Theta->assign(rawThetaData, rawThetaData + resoluation);
      }
      if (Angle_Phi != nullptr){
        auto pcsPhiData = m_PcsSensorUnit->angleOutput()[1]; //Elevation
        auto rawPhiData = const_cast<float*>(pcsPhiData.data<float>());
        Angle_Phi->assign(rawPhiData, rawPhiData + resoluation);
      }
	}
	/* Demo
	std::vector<uint8_t> ptCld;
	std::vector<float> X;
	std::vector<float> Y;
	std::vector<float> Z;
	std::vector<float> I;
	
	for (int i = 0; i < res_x * res_y; i++)
	{
		auto R = rawRangeData[i];
		auto theta = -1 * rawThetaData[i];
		auto phi = rawPhiData[i];

		auto x_tmp = R * std::cos(theta) * std::cos(phi);
		auto y_tmp = R * std::sin(theta) * std::cos(phi);
		auto z_tmp = R * std::sin(phi);
		auto i_tmp = rawIntensity[i];

		if (std::isinf(R))
		{
			x_tmp = NAN;
			y_tmp = NAN;
			z_tmp = NAN;
			i_tmp = NAN;
		}

		else
		{

			x_tmp = R * std::cos(theta) * std::cos(phi);
			y_tmp = R * std::sin(theta) * std::cos(phi);
			z_tmp = R * std::sin(phi);
			i_tmp = rawIntensity[i];
		}
		
		X.push_back(x_tmp);
		Y.push_back(y_tmp);
		Z.push_back(z_tmp);
		I.push_back(i_tmp);
	}
	*/
}
}
}


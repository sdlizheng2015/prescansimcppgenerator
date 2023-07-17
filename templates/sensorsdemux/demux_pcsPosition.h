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


#include "prescan/api/Pcs.hpp"
#include "prescan/sim/PcsSensorUnit.hpp"
#include <vector>

using prescan::sim::PcsSensorUnit;
using prescan::api::pcs::PcsSensor;



namespace prescan{
namespace sensorDemux{
  void demux_pcsPosition(PcsSensorUnit *m_pcsSensorUnit, PcsSensor & pcsSensor,
                         bool isPowerOn = false, bool isIDOn = false,
                         std::shared_ptr<std::vector<float>> X = nullptr, 
							           std::shared_ptr<std::vector<float>> Y = nullptr,
							           std::shared_ptr<std::vector<float>> Z = nullptr,
                         std::shared_ptr<std::vector<float>> I = nullptr,
                         std::shared_ptr<std::vector<int32_t>> ID = nullptr){
    if (X == nullptr && Y == nullptr && Z == nullptr && I == nullptr && ID == nullptr ){
      return;
    }
    auto res_x = pcsSensor.resolutionX();
    auto res_y = pcsSensor.resolutionY();
    auto resoluation = res_x * res_y;

    auto pcsPos = m_pcsSensorUnit->positionOutput();
    
    if (X != nullptr){
      auto pcsX = const_cast<float *>(pcsPos.at(0).data<float>());
      X->assign(pcsX, pcsX + resoluation);
    }
    if (Y != nullptr){
      auto pcsY = const_cast<float *>(pcsPos.at(1).data<float>());
      Y->assign(pcsY, pcsY + resoluation);
    }
    if (Z != nullptr){
      auto pcsZ = const_cast<float *>(pcsPos.at(2).data<float>());
      Z->assign(pcsZ, pcsZ + resoluation);
    }
    if (isPowerOn && I != nullptr){
      auto pcsPower = m_pcsSensorUnit->intensityOutput();
      auto pcsI = const_cast<float *>(pcsPower.data<float>());
      I->assign(pcsI, pcsI + resoluation);
    }
    if (isIDOn && ID != nullptr){
      auto pcsID = m_pcsSensorUnit->idOutput();
      auto p_ID = const_cast<int32_t *>(pcsID.data<int32_t>());
      ID->assign(p_ID, p_ID + resoluation);
    }


	/* Demo
    std::vector<float> points;
    points.reserve(4 * res_x * res_y);

    for (int i = 0; i < res_x * res_y; i++){
      if (isinf(pcsX[i])){
		pcsX[i] = NAN;
		pcsY[i] = NAN;
		pcsZ[i] = NAN;
		pcsI[i] = 0;
      }
	  points.push_back(pcsX[i]);
	  points.push_back(pcsY[i]);
	  points.push_back(pcsZ[i]);
	  points.push_back(pcsI[i]);
      }
    }
	*/
  }
}
}

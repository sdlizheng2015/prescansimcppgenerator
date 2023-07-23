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


#include "prescan/sim/SelfSensorUnit.hpp"
#include "prescan/sim/AmesimVehicleDynamicsUnit.hpp"

using prescan::sim::SelfSensorUnit;
using prescan::sim::AmesimVehicleDynamicsUnit;
#include <vector>

namespace prescan{
namespace sensorDemux{
  void demux_selfState(SelfSensorUnit* m_egoSelfUnit){
      double lat = m_egoSelfUnit->selfSensorOutput().Lat;
      double lng = m_egoSelfUnit->selfSensorOutput().Long;
      double alt = m_egoSelfUnit->selfSensorOutput().Alt;
	  
  }
}
}

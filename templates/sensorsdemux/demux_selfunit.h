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

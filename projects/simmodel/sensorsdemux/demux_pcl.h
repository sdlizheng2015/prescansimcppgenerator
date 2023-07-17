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
#include <prescan/api/Pointcloudlidar.hpp>
#include <prescan/sim/PointCloudLidarSensorUnit.hpp>
#include <prescan/sim/SignalView.hpp>
#include "prescan/sim/ISimulationModel.hpp"

namespace prescan{
namespace sensorDemux{

void demux_pcl(prescan::sim::ISimulationModel * simmodel, 
			 prescan::api::pointcloudlidar::PointCloudLidarSensor & pcl_sensor, 
             prescan::sim::pointcloudlidar::PointCloudUnit* _pointCloudUnit,
			 prescan::sim::pointcloudlidar::ObjectIdUnit* _objectIdUnit, 
			 prescan::sim::pointcloudlidar::PowerUnit* _powerUnit = nullptr,
             std::shared_ptr<std::vector<float>> X = nullptr, 
			 std::shared_ptr<std::vector<float>> Y = nullptr,
			 std::shared_ptr<std::vector<float>> Z = nullptr,
             std::shared_ptr<std::vector<float>> I = nullptr,
             std::shared_ptr<std::vector<int32_t>> ID = nullptr
			 ){
	static std::vector<float> _powerData;
	static std::vector<float> _objectIdData;
	static int PCL_outmode = 0;
	static std::vector<float> _Xdata;
	static std::vector<float> _Ydata;
	static std::vector<float> _Zdata;

    if (X == nullptr && Y == nullptr && Z == nullptr && I == nullptr && ID == nullptr ){
      return;
    }

    auto _isEnabledPowerCalculation = pcl_sensor.powerCalculation();
  
    if (_isEnabledPowerCalculation && I != nullptr){
	  _powerUnit->apply(prescan::sim::pointcloudlidar::PowerUnit::FuncType([simmodel](const SignalView<float> & signal) {
	    auto maxBufferSize = int(signal.size() / 3); // number of cloud points
	    _powerData.clear();
	    for (int i = 0; i < maxBufferSize; i++) {
		  _powerData.push_back(signal[i + PCL_outmode * maxBufferSize]);
        }	  
      }));
      I->assign(_powerData.data(), _powerData.data() + _powerData.size());
    }
	
	if (ID != nullptr){
      _objectIdUnit->apply(
        prescan::sim::pointcloudlidar::ObjectIdUnit::FuncType([simmodel](const SignalView<int32_t>& signal) {
	      _objectIdData.clear();
          for (auto val : signal) { 
			_objectIdData.push_back(val); 
		  }
	  }));
	  ID->assign(_objectIdData.data(), _objectIdData.data() + _objectIdData.size());
	}

  	_pointCloudUnit->apply(prescan::sim::pointcloudlidar::PointCloudUnit::FuncType(
	  [simmodel](const SignalView<float> & signal) {
	
		auto maxBufferSize = int(signal.size() / 9);
		//std::cout << "cloud points: " << maxBufferSize << std::endl;
		_Xdata.clear();
		_Ydata.clear(); 
		_Zdata.clear();
		for (int i = 0; i < maxBufferSize; i++) {
			_Xdata.push_back(signal[i * 3 + 0 + PCL_outmode * maxBufferSize]);
			_Ydata.push_back(signal[i * 3 + 1 + PCL_outmode * maxBufferSize]);
			_Zdata.push_back(signal[i * 3 + 2 + PCL_outmode * maxBufferSize]);
		}
    }));
	if (X != nullptr){
		X->assign(_Xdata.data(), _Xdata.data() + _Xdata.size());
	}
	if (Y != nullptr){
		Y->assign(_Ydata.data(), _Ydata.data() + _Ydata.size());
	}
	if (Z != nullptr){
		Z->assign(_Zdata.data(), _Zdata.data() + _Zdata.size());
	}
}
}
}
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
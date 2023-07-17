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


#include <prescan/api/Fullwaveformlidar.hpp>
#include <prescan/sim/FullWaveformLidarSensorUnit.hpp>
#include <prescan/sim/SignalView.hpp>
#include "prescan/sim/ISimulationModel.hpp"
#include <vector>

namespace prescan{
namespace sensorDemux{

void demux_fwl(prescan::sim::ISimulationModel * simmodel, 
               prescan::api::fullwaveformlidar::FullWaveformLidarSensor & sensor,
			         prescan::sim::fullwaveformlidar::PointCloudUnit* _pointCloudUnit,
               prescan::sim::fullwaveformlidar::OutputInfoUnit* _outputInfoUnit, 
			         prescan::sim::fullwaveformlidar::BeamUnit* _beamUnit, 
			         prescan::sim::fullwaveformlidar::PowerUnit* _powerUnit = nullptr,
               std::shared_ptr<std::vector<float>> X = nullptr, 
			         std::shared_ptr<std::vector<float>> Y = nullptr,
			         std::shared_ptr<std::vector<float>> Z = nullptr,
               std::shared_ptr<std::vector<float>> I = nullptr,
               std::shared_ptr<std::vector<std::vector<PRESCAN_FULLWAVEFORMLIDAR_BEAM_DATA>>> Beam = nullptr,
               std::shared_ptr<std::vector<PRESCAN_FULLWAVEFORMLIDAR_OUTPUT_INFO_DATA>> Info = nullptr
               ){

  if (X == nullptr && Y == nullptr && Z == nullptr && I == nullptr && Beam == nullptr &&  Info == nullptr){
      return;
  }
	 
  // static std::vector<float> _pointCloudData;
  static std::vector<float> _powerData;
  static std::vector<std::vector<PRESCAN_FULLWAVEFORMLIDAR_BEAM_DATA>> _beamData;
  static std::vector<PRESCAN_FULLWAVEFORMLIDAR_OUTPUT_INFO_DATA> _outputInfo;
  static int PCL_outmode = 0;
	static std::vector<float> _Xdata;
	static std::vector<float> _Ydata;
	static std::vector<float> _Zdata;

  auto _isEnabledPowerCalculation = sensor.powerCalculation();

  if (Beam != nullptr){
    _beamUnit->apply([simmodel](const SignalView<PRESCAN_FULLWAVEFORMLIDAR_BEAM_DATA>& signal) {
      std::vector<PRESCAN_FULLWAVEFORMLIDAR_BEAM_DATA> newData;
      _beamData.clear();
      for (const auto& val : signal) newData.push_back(val);
      _beamData.push_back(std::move(newData));
    });
    Beam->assign(_beamData.data(), _beamData.data() + _beamData.size());
  }


  if (Info != nullptr){
    _outputInfoUnit->apply([simmodel](const SignalView<PRESCAN_FULLWAVEFORMLIDAR_OUTPUT_INFO_DATA>& signal) {
      if (signal.size() != 1) throw std::runtime_error("Signal should always contain one output info.");
      _outputInfo.clear();
      _outputInfo.push_back(signal[0]);
    });
    Info->assign(_outputInfo.data(), _outputInfo.data() + _outputInfo.size());
  }


  if (_isEnabledPowerCalculation && I != nullptr){
      _powerUnit->apply(prescan::sim::fullwaveformlidar::PowerUnit::FuncType(
        [simmodel](const SignalView<float>& signal) { _powerData = std::vector<float>(signal.begin(), signal.end()); }));
      I->assign(_powerData.data(), _powerData.data() + _powerData.size());
  }

  _pointCloudUnit->apply(prescan::sim::fullwaveformlidar::PointCloudUnit::FuncType(
    [simmodel](const SignalView<float>& signal) { 
      // _pointCloudData = std::vector<float>(signal.begin(), signal.end());
      auto maxBufferSize = int(signal.size() / 9);
      std::cout << "cloud points: " << maxBufferSize << std::endl;
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
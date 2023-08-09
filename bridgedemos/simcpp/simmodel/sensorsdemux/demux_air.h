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


#include "prescan/api/Air.hpp"
#include "prescan/sim/AirSensorUnit.hpp"
#include "vector"
#include <cmath>

using namespace std;

namespace prescan {
  namespace sensorDemux {
    void demux_air(const prescan::sim::AirSensorUnit* sensorUnit, 
         std::shared_ptr<std::vector<double>> Range = nullptr,
         std::shared_ptr<std::vector<double>> Theta = nullptr,
         std::shared_ptr<std::vector<double>> Phi = nullptr,
         std::shared_ptr<std::vector<unsigned int>> ID = nullptr,
         std::shared_ptr<std::vector<double>> Velocity = nullptr,
         std::shared_ptr<std::vector<double>> Heading = nullptr) {
        if (Range != nullptr){
            Range->clear();
          }
        if (Theta != nullptr){
            Theta->clear();
          }
        if (Phi != nullptr){
            Phi->clear();
          }
        if (ID != nullptr){
            ID->clear();
          }
        if (Velocity != nullptr){
            Velocity->clear();
          }
        if (Heading != nullptr){
            Heading->clear();
        }

        for (const PRESCAN_AIRSENSORDATA* sensorItem : sensorUnit->airSensorOutput()) {
          if (Range != nullptr){
            Range->push_back(sensorItem->Range);
          }
          if (Theta != nullptr){
            Theta->push_back(sensorItem->Azimuth);
          }
          if (Phi != nullptr){
            Phi->push_back(sensorItem->Elevation);
          }
          if (ID != nullptr){
            ID->push_back(sensorItem->ID);
          }
          if (Velocity != nullptr){
            Velocity->push_back(sensorItem->Velocity);
          }
          if (Heading != nullptr){
            Heading->push_back(sensorItem->Heading);
          }
          if (Heading != nullptr){
            Heading->push_back(sensorItem->Heading);
          }
        }
      }
    }
  }

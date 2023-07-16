#pragma once


#include "prescan/api/Air.hpp"
#include "prescan/sim/AirSensorUnit.hpp"
#include "vector"
#include <cmath>
#include "corecrt_math_defines.h"

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

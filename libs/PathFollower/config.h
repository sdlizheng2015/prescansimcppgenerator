#pragma once
#include <iostream>

using namespace std;

namespace prescan{
namespace api{
namespace conf{

const uint32_t startindex = 0;

const uint32_t PIDEndindex = 10;

//{0kph  0.1kph   5kph    10kph   15kph   20kph   30kph   40kph   50kph   60kph   70kph};


double act_spd[PIDEndindex + 1]; // [mps]
       
double str_kps[PIDEndindex + 1];
double str_kis[PIDEndindex + 1];
double str_kds[PIDEndindex + 1];
  
double dri_kps[PIDEndindex + 1];
double dri_kis[PIDEndindex + 1];
double dri_kds[PIDEndindex + 1];

double * SpeedPIDs[7] = {act_spd, str_kps, str_kis, str_kds, dri_kps, dri_kis, dri_kds};


const uint32_t SpeedEndindex = 19;
uint32_t ActSpeedEndindex = 0;
double simulationTime[SpeedEndindex + 1];
double desiredSpeeds[SpeedEndindex + 1];
double * DesTimeSpeeds[2] = {simulationTime, desiredSpeeds};

}
}
} // namespace prescan
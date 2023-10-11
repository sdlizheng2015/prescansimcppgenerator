#pragma once


#include <cmath>
#ifdef _WIN64
#include "corecrt_math_defines.h"
#endif

#include <vector>

using namespace std;

namespace prescan{
namespace api{
namespace Preview{
namespace types{


typedef struct _PreviewConfig{
    double PreTime;
}PreviewConfig, *p_PreviewConfig;


typedef struct _PreviewResults{
    double PreVel;
    double PreAcc;
    double CurVel;
    double CurPos;
    double RTVel;
    double RTAcc;
    double PreLatError;
    double ActLatError;
    double PreHDError;
    double ActHDError;
    double PreLength;
    uint32_t posIndex;
    uint32_t preIndex;
    bool posFailed;
    bool preFailed;
    double posRatio;
    double preRatio;
    
    _PreviewResults(): posIndex(0){

    }
}PreviewResults, *p_PreviewResults;


typedef struct _Coordinate{
    double X;  // [m]
    double Y;  // [m]
    double Z;  // [m]
    double HD; // [rad]

    _Coordinate(){
        X = 0.0;
        Y = 0.0;
        Z = 0.0;
        HD = 0.0;
    }

    _Coordinate(double x, double y, double z, double h){
        X = x;
        Y = y;
        Z = z;
        HD = h;
    }
}Coordinate, *p_Coordinate;

typedef struct _VehicleControl{
    double Throttle;
    double Brake;
    double SteeringWheelAngle;
    double DesiredVelocity;
    int Gear;

    _VehicleControl(){
        Throttle = 0;
        Brake = 0;
        SteeringWheelAngle = 0;
        DesiredVelocity = 0;
        int Gear = 0;
    }
}VehicleControl, *p_VehicleControl;


typedef struct _ISPIDConfig{
    double Kp;
    double Ki;
    double Kd;
    double upper;
    double lower;
    double sampleTime; // [s]
    double Threshold;
    double Error;
    double LastError;
    double Sumerror;

    _ISPIDConfig(){
        Kp = 0;
        Ki = 0;
        Kd = 0;
        upper = 0;
        lower = 0;
        sampleTime = 0;
        Threshold = 0;
        Error = 0;
        LastError = 0;
        Sumerror = 0;
    }
}ISPIDConfig, *p_ISPIDConfig;


typedef struct _Trajectory
{
    uint32_t LocatePoints;
    uint32_t PreviewPoints;
    vector<double> trj_x; // x position [m]
    vector<double> trj_y; // y position [m]
    vector<double> trj_z; // z position [m]
    vector<double> trj_v; // velocity [m/s]
    vector<double> trj_a; // acceleration[m/s^2]
    vector<double> trj_h; // heading [rad]
    vector<double> trj_t; // duration
    vector<double> trj_s; // distance [m]
    vector<double> trj_c; // curvature [rad/m]
    vector<uint32_t> trj_cnt; // counter

    void clear(){
        trj_x.clear();
        trj_y.clear();
        trj_z.clear();
        trj_v.clear();
        trj_a.clear();
        trj_h.clear();
        trj_t.clear();
        trj_s.clear();
        trj_c.clear();
        trj_cnt.clear();
    }
}Trajectory, *p_Trajectory;

}}}}


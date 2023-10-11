#pragma once

#include "types.h"
#include "config.h"

#include <cmath>

namespace prescan{
namespace api{
namespace Utils{

using namespace std;
using namespace prescan::api::Preview::types;
using namespace prescan::api::conf;

class NonLinearBlock
{
public:
    NonLinearBlock(){
        // pass
    };

    bool DeadZone(double upper, double lower, double input){
        if(upper<= lower){
            return false; //用户上下限设置错误
        }
    else{
        if((input > lower) && (input < upper)){
            return true;
        }else return false;
    }
    };

    void Saturation(double upper, double lower, double &in_out){
        if (upper <= lower){
            in_out = lower; //用户上下限输入错误
        }else{
            if (in_out > upper){
                in_out = upper;
            }else if (in_out < lower){
                in_out =  lower;
            }else{
                //
            }
    }
    };
};


class ISPIDController
{
public:
    ISPIDController():Error(0), LastError(0), Sumerror(0){
        // pass
    }

    void control(double desired, double actual, ISPIDConfig &pidConfig, double &control){
        Error = desired - actual;

        if (abs(Error) <= pidConfig.Threshold){
            Sumerror= Sumerror + Error * pidConfig.sampleTime;
            control= pidConfig.Kp*Error + pidConfig.Ki*Sumerror + pidConfig.Kd *(Error - LastError);
        }else{
            control= pidConfig.Kp*Error + pidConfig.Kd*(Error - LastError);
        }
        
        NonLiner.Saturation(pidConfig.upper, pidConfig.lower, control);

        LastError= Error;
    };


    void ResetError(){
        LastError = 0;
        Sumerror = 0;
        Error = 0;
    };

private:
    NonLinearBlock NonLiner;
    double Error, LastError, Sumerror;
};

void lookupSpeedTable(double time, double &speed){
    /*
    */
    uint32_t IndexAct;
    if(time <= simulationTime[startindex]){
        speed = *(desiredSpeeds + startindex);
    }
    else if(time >= simulationTime[ActSpeedEndindex]){
        speed = *(desiredSpeeds + ActSpeedEndindex);
    }
    else{
        for (uint32_t i = startindex; i<= ActSpeedEndindex; i++){
            if((time >= simulationTime[i]) && (time <= simulationTime[i+1])){
                IndexAct = i;
                break;
            }
        }
        speed = ((*(desiredSpeeds + IndexAct + 1) - *(desiredSpeeds + IndexAct))
               / (simulationTime[IndexAct + 1] - simulationTime[IndexAct])
               * (time - simulationTime[IndexAct])) + *(desiredSpeeds + IndexAct);
    }
}

void lookupPIDTable(double cur_speed, double *kps, double *kis, double *kds,
                    double &kp, double &ki, double &kd){
    /*
    
    */
    int16_t IndexAct;
    if(cur_speed <= act_spd[startindex]){
        kp = *(kps + startindex);
        ki = *(kis + startindex);
        kd = *(kds + startindex);
    }
    else if(cur_speed >= act_spd[PIDEndindex]){
        kp = *(kps + PIDEndindex);
        ki = *(kis + PIDEndindex);
        kd = *(kds + PIDEndindex);
    }
    else{
        for (int i = startindex; i<= PIDEndindex; i++){
            if((cur_speed >= act_spd[i]) && (cur_speed <= act_spd[i+1])){
                IndexAct = i;
                break;
            }
        }
        kp = ((*(kps + IndexAct + 1) - *(kps + IndexAct))
               / (act_spd[IndexAct + 1] - act_spd[IndexAct])
               * (cur_speed - act_spd[IndexAct])) + *(kps + IndexAct);
        ki = ((*(kis + IndexAct + 1) - *(kis+IndexAct))
               / (act_spd[IndexAct + 1] - act_spd[IndexAct])
               * (cur_speed - act_spd[IndexAct])) + *(kis + IndexAct);
        kd = ((*(kds+IndexAct + 1) - *(kds+IndexAct))
               / (act_spd[IndexAct + 1] - act_spd[IndexAct])
               * (cur_speed - act_spd[IndexAct])) + *(kds + IndexAct);
    }
}

double Sign(double x){
    if(x >= 0) return 1;
    else return -1;
}

double change_head_range(double Yaw){
    // yaw: [rad]
    // return [-PI, PI] [rad]
    int Circle;
    double HD;
    if (Yaw == 0.0){
        Circle = 0;
    }
    else{
        Circle = (int)(Yaw / (Sign(Yaw) * 2 * M_PI));
    }

    HD = Yaw - Sign(Yaw) * (double)Circle * 2 * M_PI;

    if (HD < 0.0){
        HD = HD + 2 * M_PI;
    }
    if (HD > M_PI){
        HD = HD - 2 * M_PI;
    }
    return HD;
}


void GlobalToLocalTransfer(Coordinate &globalPoint, Coordinate &OriginalPoint, 
                                 Coordinate &localPoint){
	double inv_transMtrix[2][2];
    auto deltaX = globalPoint.X - OriginalPoint.X;
    auto deltaY = globalPoint.Y - OriginalPoint.Y;
    inv_transMtrix[0][0] = cos(OriginalPoint.HD);
    inv_transMtrix[0][1] = sin(OriginalPoint.HD);
    inv_transMtrix[1][0] =-sin(OriginalPoint.HD);
    inv_transMtrix[1][1] = cos(OriginalPoint.HD);
    localPoint.X = inv_transMtrix[0][0] * deltaX + inv_transMtrix[0][1] * deltaY;
    localPoint.Y = inv_transMtrix[1][0] * deltaX + inv_transMtrix[1][1] * deltaY;
    localPoint.HD = change_head_range(globalPoint.HD - OriginalPoint.HD);
    localPoint.Z = globalPoint.Z - OriginalPoint.Z;
}


void LocalToGlobalTransfer(Coordinate &localPoint, Coordinate &OriginalPoint, 
                           Coordinate &globalPoint){
	double transMtrix[2][2];
    transMtrix[0][0] = cos(OriginalPoint.HD);
    transMtrix[0][1] =-sin(OriginalPoint.HD);
    transMtrix[1][0] = sin(OriginalPoint.HD);
    transMtrix[1][1] = cos(OriginalPoint.HD);
    globalPoint.X = transMtrix[0][0]*localPoint.X+ transMtrix[0][1]*localPoint.Y+ OriginalPoint.X;
    globalPoint.Y = transMtrix[1][0]*localPoint.X+ transMtrix[1][1]*localPoint.Y+ OriginalPoint.Y;
    globalPoint.Z = localPoint.Z + OriginalPoint.Z;
    globalPoint.HD = change_head_range(localPoint.HD + OriginalPoint.HD);
}

void Interpolation(Coordinate &FrontPoint, Coordinate& RearPoint, Coordinate& ATPPoint, 
                     double PreLength, double &LatError, double &ratio){
    auto h1 = ATPPoint.X + PreLength - FrontPoint.X;
    auto h2 = RearPoint.X - ATPPoint.X - PreLength;

    if(RearPoint.X != FrontPoint.X){
        LatError = h2 / (h1 + h2) * (FrontPoint.Y - RearPoint.Y) 
            + RearPoint.Y - ATPPoint.Y; 
        ratio = h2 / (h1 + h2);
    }else{
        LatError = 0;
        ratio = 0;
    }
}




}}}


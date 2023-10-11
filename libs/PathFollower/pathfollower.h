#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <regex>
#include <iosfwd>
#include <regex>
#include <vector>
#include <cmath>
#include "types.h"
#include "utils.h"
#include "config.h"
#include "prescan/api/Experiment.hpp"
#include "prescan/api/Trajectory.hpp"
#include "cnpy.h"
#include "getUserData.h"

using namespace std;
using prescan::api::Preview::types::Coordinate;
using prescan::api::Preview::types::PreviewResults;
using prescan::api::Utils::GlobalToLocalTransfer;
using prescan::api::Utils::LocalToGlobalTransfer;
using prescan::api::Utils::Interpolation;
using prescan::api::Utils::change_head_range;
using prescan::api::Utils::ISPIDConfig;
using prescan::api::Utils::ISPIDController;
using prescan::api::Utils::lookupPIDTable;
using prescan::api::Utils::lookupSpeedTable;
using prescan::api::UserData::get_ego_trigger_distance;
using prescan::api::UserData::EgoUserDate;
using prescan::api::Preview::types::VehicleControl;


namespace prescan{
namespace sim{

class AmesimDynamicsPathFollowerUnit{
public:
    AmesimDynamicsPathFollowerUnit(){

    };

    void registerUnit(const prescan::api::experiment::Experiment &experiment, 
                  const string & confPath, const string & ego_name, bool savePath=false){
        
        /*Get user data*/
        get_ego_trigger_distance(experiment, egoUserDate, ego_name);

        /*Load PID and speed table*/
        LoadPIDTable(confPath);
        LoadSpeedTable(confPath);

        /*Process path data*/
        const double step = 0.1;
        
        trajectory.clear();
        auto ego = experiment.getObjectByName<prescan::api::types::WorldObject>(ego_name);
        auto trj = prescan::api::trajectory::getActiveTrajectory(ego);
        auto length = trj.path().length();
        auto pose = trj.path().poseAtDistance(0);
        OriginalPoint_inInitFrame.X = pose.position().x();
        OriginalPoint_inInitFrame.Y = pose.position().y();
        OriginalPoint_inInitFrame.Z = pose.position().z();
        OriginalPoint_inInitFrame.HD = change_head_range(pose.orientation().yaw());
        trajectory.LocatePoints = static_cast<uint32_t>(length / step) + 1;
        trajectory.PreviewPoints = trajectory.LocatePoints + 1;
        for(uint32_t i = 0; i < trajectory.LocatePoints; i++){
            auto pose = trj.path().poseAtDistance(static_cast<double>(i) * step);
            Coordinate globalPoint, localPoint;
            globalPoint.X = pose.position().x();
            globalPoint.Y = pose.position().y();
            globalPoint.Z = pose.position().z();
            globalPoint.HD = change_head_range(pose.orientation().yaw());
            GlobalToLocalTransfer(globalPoint, OriginalPoint_inInitFrame, localPoint);
            trajectory.trj_x.push_back(localPoint.X);
            trajectory.trj_y.push_back(localPoint.Y);
            trajectory.trj_z.push_back(localPoint.Z);
            trajectory.trj_h.push_back(localPoint.HD);
            trajectory.trj_s.push_back(static_cast<double>(i) * step);
        }
        AppendEndPoint();
        
        /*Find trigger*/
        if (egoUserDate.found_trigger){
            for (uint32_t i = 0; i < trajectory.LocatePoints; i++){
                if (trajectory.trj_s[i] >= egoUserDate.egoTriggerDistance){
                    egoUserDate.triggerIndex = i;
                    break;
                }
            }
        }else{
            egoUserDate.triggerIndex = trajectory.LocatePoints;
        }


        if(savePath){
            /*Please check if there is a result folder*/
            cnpy::npy_save("./path_X.npy", trajectory.trj_x.data(), { 1, trajectory.trj_x.size() }, "a");
            cnpy::npy_save("./path_Y.npy", trajectory.trj_y.data(), { 1, trajectory.trj_y.size() }, "a");
            cnpy::npy_save("./path_Z.npy", trajectory.trj_z.data(), { 1, trajectory.trj_z.size() }, "a");
            cnpy::npy_save("./path_H.npy", trajectory.trj_h.data(), { 1, trajectory.trj_h.size() }, "a");
        }
        
        /*previewResults*/
        previewResults.posIndex = 0;

        /*Initial PID config*/
        strpidConfig.sampleTime = 1.0 / static_cast<double>(experiment.scheduler().simulationFrequency());
        strpidConfig.Threshold = 10;
        strpidConfig.upper = 1.0;
        strpidConfig.lower = -1.0;
        strController.ResetError();

        dripidConfig.sampleTime = strpidConfig.sampleTime;
        dripidConfig.Threshold = 10;
        dripidConfig.upper = 1.0;
        dripidConfig.lower = 0;
        driController.ResetError();
    }

    void SeekForDesSpeed(double time, double & DesVel){
        lookupSpeedTable(time, DesVel);
    }

    bool targetTriggered(){
        return egoUserDate.triggered;
    }

    void Preview(Coordinate & globalPoint, double time, double ActVel, VehicleControl & control, bool record=false ){
        FindNearest(globalPoint);
        FindPreview(ActVel, 1);
        SteerControl(ActVel, control.SteeringWheelAngle);
        SeekForDesSpeed(time, control.DesiredVelocity);
        SpeedControl(ActVel, control.DesiredVelocity, control.Throttle);
        control.Gear = 1;
        if(previewResults.posIndex >= egoUserDate.triggerIndex){
            egoUserDate.triggered = true;
        }
        
        PosFailed(control.Throttle, control.SteeringWheelAngle, control.Brake, control.Gear);
        // cout << "Trigger: " <<egoUserDate.triggerIndex <<"  PosIndex:" << previewResults.posIndex << endl;
        // cout <<"posFailed: " << (int)previewResults.posFailed << endl;
        // cout <<"posIndex: " <<previewResults.posIndex << endl;
        // cout <<"posLatErr: " <<previewResults.ActLatError << endl;
        // cout <<"preFailed: " << (int)previewResults.preFailed << endl;
        // cout <<"PreIndex: " <<previewResults.preIndex << endl;
        // cout <<"preLatErr: " <<previewResults.PreLatError << endl;
        if (record){
            recordTime.clear();
            recordActSpd.clear();
            recordDesSpd.clear();
            recordTime.push_back(time);
            recordActSpd.push_back(ActVel);
            recordDesSpd.push_back(control.DesiredVelocity);
            cnpy::npy_save("./recordTime.npy", recordTime.data(), { 1, recordTime.size() }, "a");
            cnpy::npy_save("./recordActSpd.npy", recordActSpd.data(), { 1, recordActSpd.size() }, "a");
            cnpy::npy_save("./recordDesSpd.npy", recordDesSpd.data(), { 1, recordDesSpd.size() }, "a");
        }
    }


private:
    void LoadPIDTable(const string & confPath){
        ifstream pidConf;
        pidConf.open(confPath + "pids.conf.txt");
        regex pattern(R"(\d+\.\d+)");
        string line;
        int seq = 0;
        cout << "Load PID table: " << endl;
        while (getline(pidConf, line))
        {
            cout << line << endl;
            std::sregex_iterator iterator(line.begin(), line.end(), pattern);
            std::sregex_iterator end;

            int i = 0;
            while (iterator != end) {
                std::smatch match = *iterator;
                auto value = stod(match.str());
                prescan::api::conf::SpeedPIDs[seq][i] = value;
                ++iterator;
                i++;
            }
            seq ++;
        }
        
        pidConf.close();
    }


    void LoadSpeedTable(const string & confPath){
        ifstream speedConf;
        speedConf.open(confPath + "speeds.conf.txt");
        regex pattern(R"(\d+\.\d+)");
        string line;
        int seq = 0;
        cout << "Load Speeds table: " << endl;
        while (getline(speedConf, line))
        {
            cout << line << endl;
            std::sregex_iterator iterator(line.begin(), line.end(), pattern);
            std::sregex_iterator end;

            int i = 0;
            while (iterator != end) {
                std::smatch match = *iterator;
                auto value = stod(match.str());
                if(i == 0){
                    prescan::api::conf::DesTimeSpeeds[0][seq] = value;
                }else{
                    prescan::api::conf::DesTimeSpeeds[1][seq] = value;
                }
                
                ++iterator;
                i++;
            }
            prescan::api::conf::ActSpeedEndindex = seq ++;
        }
        speedConf.close();
        cout << "ActSpeedEndindex: " << prescan::api::conf::ActSpeedEndindex << endl;
    }

    void PosFailed(double &drive, double & steer, double & brake, int & gear){
        if(previewResults.posFailed || previewResults.preFailed){
            drive = 0;
            steer = 0;
            brake = 1;
            gear = 0;
        }else{
            brake = 0;
        }
    }

    void SteerControl(double ActVel, double &steer){
        lookupPIDTable(ActVel, prescan::api::conf::str_kps, prescan::api::conf::str_kis, prescan::api::conf::str_kds, 
                    strpidConfig.Kp, strpidConfig.Ki, strpidConfig.Kd);
        strController.control(0, -previewResults.PreLatError, strpidConfig, steer);
        steer = steer * 540.0 / 180 * M_PI;
    }
    
    void SpeedControl(double ActVel, double DesVel, double & drive){
        lookupPIDTable(ActVel, prescan::api::conf::dri_kps, prescan::api::conf::dri_kis, prescan::api::conf::dri_kds, 
                    dripidConfig.Kp, dripidConfig.Ki, dripidConfig.Kd);
        driController.control(DesVel, ActVel, dripidConfig, drive);


    }


    void FindNearest(Coordinate & globalPoint){
        previewResults.posFailed = true;
        GlobalToLocalTransfer(globalPoint, OriginalPoint_inInitFrame, RTLocalPoint_inInitFrame);
        RTLocalPoint_inInitFrame.HD = change_head_range(RTLocalPoint_inInitFrame.HD);
        RTOriginalPoint_inTrjFrame.X = 0;
        RTOriginalPoint_inTrjFrame.Y = 0;
        RTOriginalPoint_inTrjFrame.HD = RTLocalPoint_inInitFrame.HD;
        GlobalToLocalTransfer(RTLocalPoint_inInitFrame, RTOriginalPoint_inTrjFrame, RTLocalPoint_inTrjFrame);
        const uint32_t startBack = 10;
        uint32_t search_pos_index = 0;
        const uint32_t max_search_num = 2;
        uint32_t PosStartIndex = 0;
        uint32_t PosIndexes[max_search_num] = {trajectory.LocatePoints + max_search_num, trajectory.LocatePoints + max_search_num};
        Coordinate PosPoints[max_search_num][max_search_num];
        Coordinate TrjPointF, TrjPointR, RTTrjPointF, RTTrjPointR;
        
        if(previewResults.posIndex < startBack){
            PosStartIndex = 0;
        }else{
            PosStartIndex = previewResults.posIndex - startBack;
        }

        if (trajectory.LocatePoints >= 1){
            for(uint32_t index = PosStartIndex; index < trajectory.LocatePoints - 2; index++){
                if (index == 0){
                    TrjPointF.X = trajectory.trj_x[index] - 0.1;
                }else{
                    TrjPointF.X = trajectory.trj_x[index];
                }
                TrjPointF.Y = trajectory.trj_y[index];
                TrjPointF.HD = 0;
                GlobalToLocalTransfer(TrjPointF, RTOriginalPoint_inTrjFrame, RTTrjPointF);

                TrjPointR.X = trajectory.trj_x[index + 1];
                TrjPointR.Y = trajectory.trj_y[index + 1];
                TrjPointR.HD = 0;
                GlobalToLocalTransfer(TrjPointR, RTOriginalPoint_inTrjFrame, RTTrjPointR);
                
                if ((RTTrjPointF.X - RTLocalPoint_inTrjFrame.X) * (RTTrjPointR.X - RTLocalPoint_inTrjFrame.X) <= 0){
                    PosIndexes[search_pos_index] = index;
                    PosPoints[search_pos_index][0] = RTTrjPointF;
                    PosPoints[search_pos_index][1] = RTTrjPointR;
                    search_pos_index ++;
                    if (search_pos_index >= max_search_num){
                        break;
                    }
                }

            }
        }

        if(PosIndexes[0] < trajectory.LocatePoints - 1){
            search_pos_index = 0;
            previewResults.posIndex = PosIndexes[search_pos_index];
            previewResults.posFailed = false;
        }
        if (PosIndexes[1] < trajectory.LocatePoints - 1){
            auto delta_head1 = abs(RTLocalPoint_inInitFrame.HD - trajectory.trj_h[PosIndexes[0]]);
            auto delta_head2 = abs(RTLocalPoint_inInitFrame.HD - trajectory.trj_h[PosIndexes[1]]);
            if (delta_head1 > M_PI){
                delta_head1 = 2 * M_PI - delta_head1;
            }
            if (delta_head2 > M_PI){
                delta_head2 = 2 * M_PI - delta_head2;
            }
            if(delta_head1 < delta_head2){
                search_pos_index = 0;
            }else{
                search_pos_index = 1;
            }
            previewResults.posIndex = PosIndexes[search_pos_index];
        }

        if(!previewResults.posFailed){
            Interpolation(PosPoints[search_pos_index][0], PosPoints[search_pos_index][1],
                          RTLocalPoint_inTrjFrame, 0.0, previewResults.ActLatError, previewResults.posRatio);
            previewResults.ActHDError = trajectory.trj_h[previewResults.posIndex] - RTLocalPoint_inInitFrame.HD;
        }
    }

    void FindPreview(double ActVel, int32_t MoveDirFlag){
        previewResults.preFailed = true;
        previewResults.preIndex = 0;
        previewResults.PreLatError = 0;
        previewResults.PreLength = 0;

        const double preTime = 0.8;
        const double minPreLength = 1;
        const double minActVel = 1;

        if(!previewResults.posFailed){

            Coordinate TrjPointF, TrjPointR, RTTrjPointF, RTTrjPointR;
            if (trajectory.PreviewPoints < 2){
                previewResults.preFailed = true;
            }else{
                for(uint32_t index = previewResults.posIndex; index < trajectory.PreviewPoints - 2; index ++){
                    TrjPointF.X = trajectory.trj_x[index];
                    TrjPointF.Y = trajectory.trj_y[index];
                    TrjPointF.HD = 0;
                    GlobalToLocalTransfer(TrjPointF, RTOriginalPoint_inTrjFrame, RTTrjPointF);

                    TrjPointR.X = trajectory.trj_x[index + 1];
                    TrjPointR.Y = trajectory.trj_y[index + 1];
                    TrjPointR.HD = 0;
                    GlobalToLocalTransfer(TrjPointR, RTOriginalPoint_inTrjFrame, RTTrjPointR);

                    if(ActVel < minActVel){
                        previewResults.PreLength = minPreLength;
                    }else{
                        previewResults.PreLength = ActVel * preTime * static_cast<double>(MoveDirFlag);
                    }
                    if ((RTTrjPointF.X - RTLocalPoint_inTrjFrame.X - previewResults.PreLength) * 
                        (RTTrjPointR.X - RTLocalPoint_inTrjFrame.X  - previewResults.PreLength) <= 0){
                        previewResults.preFailed = false;
                        previewResults.preIndex = index;
                        Interpolation(RTTrjPointF, RTTrjPointR, RTLocalPoint_inTrjFrame, previewResults.PreLength, 
                                      previewResults.PreLatError, previewResults.preRatio);
                        break;
                    }else{
                        previewResults.preFailed = true;
                    }
                }
            }
        }
    }


    void AppendEndPoint(){
        const double AppendLength = 20;
        Coordinate Original, localPoint, globalPoint;
        Original.X = trajectory.trj_x[trajectory.LocatePoints - 1];
        Original.Y = trajectory.trj_y[trajectory.LocatePoints - 1];
        Original.HD = trajectory.trj_h[trajectory.LocatePoints - 1];

        localPoint.X = AppendLength;
        localPoint.Y = 0.0;
        localPoint.HD = Original.HD;
        LocalToGlobalTransfer(localPoint, Original, globalPoint);
        trajectory.trj_x.push_back(globalPoint.X);
        trajectory.trj_y.push_back(globalPoint.Y);
        trajectory.trj_h.push_back(Original.HD);
    }
    
    Coordinate OriginalPoint_inInitFrame;
    Coordinate RTLocalPoint_inInitFrame;
    Coordinate RTOriginalPoint_inTrjFrame;
    Coordinate RTLocalPoint_inTrjFrame;


    prescan::api::Preview::types::Trajectory trajectory;
    PreviewResults previewResults;
    ISPIDController strController;
    ISPIDConfig strpidConfig;
    ISPIDController driController;
    ISPIDConfig dripidConfig;
    vector<double> recordActSpd;
    vector<double> recordDesSpd;
    vector<double> recordTime;
    EgoUserDate egoUserDate;

};
}
}

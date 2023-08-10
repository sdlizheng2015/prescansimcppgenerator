#include "bridge/bridge_base/bridge_shm.hpp"
#include "bridge/bridge_base/bridge_base.hpp"
#include "bridge/bridge_msgs/basic_msgs/header.pb.h"
#include "bridge/bridge_msgs/chassis_msgs/chassis.pb.h"
#include "bridge/bridge_msgs/control_msgs/control_cmd.pb.h"
#include "bridge/bridge_msgs/localiztion_msgs/localization.pb.h"
#include "bridge/bridge_msgs/perception_msgs/obstacle.pb.h"

using namespace prescan::bridge;
using namespace prescan::common;
using namespace prescan::chassis;
using namespace prescan::control;
using namespace prescan::localization;
using namespace prescan::perception;

int main(int argc, char*argv[]){
    double _timeout{0.001};
    bool _block{true};

    #if __linux__
        ThreadPool threadpool{10};
    #endif

    prescan::bridge::HandlerManager hander_manager;

    prescan::bridge::ShmHandler syncHandler{"Sync", 1024};
    Header syncheader;

    prescan::bridge::ShmHandler chassisHandler{"Chassis", 1024};
    Chassis chassis;

    prescan::bridge::ShmHandler localizationHandler{"Localization", 1024};
    LocalizationStatus localization;

    prescan::bridge::ShmHandler obstaclesHandler{"Obstacles", 1024};
    Obstacles obstacles;

    prescan::bridge::ShmHandler polyLanesHandler{"PolyLanes", 1024 * 5};
    PolyLanes polyLanes;

    prescan::bridge::ShmHandler pointLanesHandler{"PointLanes", 1024 * 10};
    PointLanes pointLanes;

    prescan::bridge::ShmHandler controlHandler{"Control", 1024};
    ControlCommand control;

    #if __linux__
        threadpool.start();
    #endif

    hander_manager.add_handler(&syncHandler);
    hander_manager.add_handler(&chassisHandler);
    hander_manager.add_handler(&localizationHandler);
    hander_manager.add_handler(&obstaclesHandler);
    hander_manager.add_handler(&polyLanesHandler);
    hander_manager.add_handler(&pointLanesHandler);
    hander_manager.add_handler(&controlHandler);
    hander_manager.initial_shmhandlers(true);

    while (true)
    {
        /*read data from Prescan*/
        syncHandler.read_shm(p_header, &syncheader, _timeout, _block);
        if(syncheader.timestamp_sec() < 0) break;

        #if __linux__
            /*This threadpool works correctly on linux, not always on Windows*/
            threadpool.appendTask(std::bind(&ShmHandler::read_shm, &chassisHandler, p_header, &chassis, _timeout, _block));
            threadpool.appendTask(std::bind(&ShmHandler::read_shm, &localizationHandler, p_header, &localization, _timeout, _block));
            threadpool.appendTask(std::bind(&ShmHandler::read_shm, &obstaclesHandler, p_header, &obstacles, _timeout, _block));
            threadpool.appendTask(std::bind(&ShmHandler::read_shm, &polyLanesHandler, p_header, &polyLanes, _timeout, _block));
            threadpool.appendTask(std::bind(&ShmHandler::read_shm, &pointLanesHandler, p_header, &pointLanes, _timeout, _block));
            threadpool.wait();
        #endif

        #if _WIN64
            chassisHandler.read_shm(p_header, &chassis, _timeout, _block);
            localizationHandler.read_shm(p_header, &localization, _timeout, _block);
            obstaclesHandler.read_shm(p_header, &obstacles, _timeout, _block);
            polyLanesHandler.read_shm(p_header, &polyLanes, _timeout, _block);
            pointLanesHandler.read_shm(p_header, &pointLanes, _timeout, _block);
        #endif
        
        /*Do your algorithm here*/
        control.set_brake(0);
        control.set_throttle(1);
        control.set_steering(0);
        control.set_gear_location(Chassis::GearPosition::Chassis_GearPosition_GEAR_REVERSE);

        /*Send data to Prescan*/
        controlHandler.write_shm(p_header, &control, _timeout, _block);
    }
    hander_manager.free_shmhandlers();

    #if __linux__
        threadpool.stop();
    #endif

    return 1;
}
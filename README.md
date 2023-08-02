<div style="text-align: center"><span style="font-family: Arial; font-size: 20px;"><strong>—————————————————————————————</strong></span></div>
<div style="text-align: center"><span style="font-family: Arial; font-size: 20px;"><strong>Welcome to PrescanSimCppGenerator 1.0</strong></span></div>
<div style="text-align: center"><span style="font-family: Arial; font-size: 20px;"><strong>—————————————————————————————</strong></span></div>

---

[中文文档](/README_cn.md)

[toc]

# Introduction
Prescan Simcpp Generator aims to provide a auto-generator for Prescan SimCPP project so that user can easily generate the C++ codes, build and run the simulation. Now, the following parts are supported for generation:
<div id="introduction"></div>

- Air Sensor
- Analytic Lane Marker Sensor
- Amesim Preconfigured Dynamics
- Detailed Camera
- ISimulationModel
- Lane Marker Sensor
- ObjectListProviderUnit
- Point Cloud Sensor
- Pyhsics-Based Fullwaveform Lidar
- Physics-Based Camera(Unreal)
  - [x] RGB
  - [ ] Control
  - [ ] Distance
  - [ ] Exposure
  - [ ] Illuminance
  - [ ] OptixFlow
  - [ ] RainOverlay  
- Pyhsics-Base Point Cloud Lidar
- StateActuatorUnit
- SelfSensorUnit
- Trajectory

# Version

1. The project is for Prescan 2302 and higher version because the C++ namespace is changed after Prescan 2211.
2. The project is developed and tested based on Prescan 2302 with full functionalities(all features enabled).
3. The generator supports both Windows and Ubuntu.
4. For those Prescan with some feature disabled, user may need to do some modifications to the generator source codes before you can correctly use it.

# Statement
This project is not officially released by Siemens Prescan, which is only for user learning and reference.

# Support
==**NOTE**==, for any issues you meet with when using this generator, please firstly contact the Prescan support engineer or you can submit your issue on Siemens Support Center. Please DON'T send any emails to Prescan official Support Team directly, otherwise you won't have any response to your question. 


# GetStart
## Prerequisites
Before you start with this project, make sure you've installed the following softwares:
* Prescan 2302 or higher
* Python 3.8 or higher，with following packages：
  * pyyaml
* C/C++ compiler(recommended to use Visual Studio 2019)
* CMake 3.21 or higher

## Environment Vars
### Windows
According to installation of Prescan on your machine, you may need to do the following configurations：
1. First change the vars in ```set_env.bat```，add the Prescan bin directory as well as Plugins bin if exists to system ```PATH```；
2. Set Prescan CMake path to ```Prescan_DIR```；
3. If there is no python interpreter in your system path, you may need to specify a python by adding it to ```PATH```. If you have python in system path, please ignore and delete this step.
4. Add Prescan python/modules/plugins directories to ```PYTHONPATH```, which you can find in Prescan installation folder；
5. Set the Prescan experiment directory to ```experiment_dir```;
6. Set the Prescan experiment pb file path to ```experiment_pb```;
7. ```simcpp_dir``` is the destination foler where you want to put the generated simcpp project. Typically, you can set it to a 'simcpp' folder in the ```%experiment_dir%```. So set it to ```"%experiment_dir%\\simcpp"```

Finally it looks like this：

```powershell
@ECHO OFF
setlocal

set "PATH=D:\Simcenter Prescan\Prescan_2302\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\FullWaveformLidarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\PBRadarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\PointCloudLidarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\ProbabilisticSensorsPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\V2XPlugin\bin;%PATH%"

set "Prescan_DIR=D:\Simcenter Prescan\Prescan_2302\lib\cmake"

set "PATH=D:\Python38\Scripts\;D:\Python38\;%PATH%"

set "PYTHONPATH=D:\Simcenter Prescan\Prescan_2302\python;D:\Simcenter Prescan\Prescan_2302\modules;D:\Simcenter Prescan\Prescan_2302\Plugins;%PYTHONPATH%"

set experiment_dir="C:\\Users\\yiyan5ez\\Desktop\\PrescanSimcppGenerator\\prescandemos\\SimcppGenerator"
set experiment_pb="C:\\Users\\yiyan5ez\\Desktop\\PrescanSimcppGenerator\\prescandemos\\SimcppGenerator\\SimcppGenerator.pb"
set simcpp_dir="C:\\Users\\yiyan5ez\\Desktop\\PrescanSimcppGenerator\\prescandemos\\SimcppGenerator\\simcpp"

echo set path ...
cmd
```

### Ubuntu
==**NOTE**==：Prescan 2307 or higher offcially supports Ubuntu 20.04 and has almost all the features that it has on Windows except V2X plugin. For the previous version(<2307), Prescan doesn't support Physics-Based Point Cloud Lidar and Physics-Based Radar on Ubuntu 20.04. In this situation, user may need to add plugin path to environment vars according to the Prescan installation on your own Ubuntu machine. With regard to how to add Prescan Plugin to PATH，please refer to Prescan help manual. In this demo, we don't have any plugins installed on Ubuntu.

```bash
#! /usr/bin/env bash

# add prescan
export Prescan_version=Prescan_2302
export PATH=$PATH:/usr/local/${Prescan_version}/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/${Prescan_version}/lib

# add Prescan Plugin path
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/${Prescan_version}/<path_to_plugin>/bin

export Prescan_DIR=/usr/local/${Prescan_version}/lib/cmake

export PYTHONPATH=$PYTHONPATH:/usr/local/${Prescan_version}/python:/usr/local/${Prescan_version}/modules:/usr/local/${Prescan_version}/Plugins

export experiment_dir=/home/yang/Desktop/PrescanSimcppGenerator/prescandemos/SimcppGenerator
export experiment_pb=/home/yang/Desktop/PrescanSimcppGenerator/prescandemos/SimcppGenerator/SimcppGenerator.pb
export simcpp_dir=/home/yang/Desktop/PrescanSimcppGenerator/prescandemos/SimcppGenerator/simcpp
```


## Codes Modification
For users who have installed Prescan with all features enabled, you can directly use this generator. However, if some features are disabled, you may need to do some modifications to the generator source codes before you can use it correctly. If you have all features enabled, you will find a Plugins folder in Prescan installation path, as shown in following picture. These highlighted plugins have some open API to users.
![plugins](./pic/plugins.png)

### prescan_python_dmapi.py
Firstly, you need to delete/commentout some codes in ```./rename_api_namespace/prescan_python_dmapi.py```. This python module will import the open API of all Prescan features.

```python
import prescan.api
import prescan.api.experiment
import prescan.api.types
import prescan.api.log
import prescan.genericmodel
import prescan.internal
import prescan.assets
import prescan.assets.bufferdata
import prescan.api.air
import prescan.api.alms
import prescan.api.annotations
import prescan.api.brs
import prescan.api.camera
import prescan.api.depthcamera
import prescan.api.iss
import prescan.api.collisiondetection
import prescan.api.communication
import prescan.api.roads
import prescan.api.lidar
import prescan.api.lms
import prescan.api.objectlistprovider
import prescan.api.ocs
import prescan.api.opendrive
import prescan.api.openscenario
import prescan.api.pcs
import prescan.api.physicsbasedcameraunreal
import prescan.api.viewer
import prescan.api.radar
import prescan.api.tis
import prescan.api.trafficsignalsensor
import prescan.api.scenario
import prescan.api.trajectory
import prescan.api.ultrasonic
import prescan.api.userdata
import prescan.api.vehicledynamics
import prescan.internal.algorithm
import prescan.internal.scenario
import prescan.api.fullwaveformlidar
import prescan.api.pbradar
import prescan.api.pointcloudlidar
import prescan.api.probabilisticcamera
import prescan.api.probabilisticradar
import prescan.api.v2x
import prescan.api.roads.types
import prescan.api as prescan_api
import prescan.api.experiment as prescan_api_experiment
import prescan.api.types as prescan_api_types
import prescan.api.log as prescan_api_log
import prescan.genericmodel as prescan_genericmodel
import prescan.internal as prescan_internal
import prescan.assets as prescan_assets
import prescan.assets.bufferdata as prescan_assets_bufferdata
import AIR.python_interface.prescan.api.air as prescan_api_air
import ALMS.python_interface.prescan.api.alms as prescan_api_alms
import Annotations.python_interface.prescan.api.annotations as prescan_api_annotations
import BRS.python_interface.prescan.api.brs as prescan_api_brs
import Camera.python_interface.prescan.api.camera as prescan_api_camera
import Camera.python_interface.prescan.api.depthcamera as prescan_api_depthcamera
import Camera.python_interface.prescan.api.iss as prescan_api_iss
import CollisionDetection.python_interface.prescan.api.collisiondetection as prescan_api_collisiondetection
import Communication.python_interface.prescan.api.communication as prescan_api_communication
import GraphBasedRoads.python_interface.prescan.api.roads as prescan_api_roads
import Lidar.python_interface.prescan.api.lidar as prescan_api_lidar
import LMS.python_interface.prescan.api.lms as prescan_api_lms
import ObjectListProvider.python_interface.prescan.api.objectlistprovider as prescan_api_objectlistprovider
import OCS.python_interface.prescan.api.ocs as prescan_api_ocs
import OpenDriveImporter.python_interface.prescan.api.opendrive as prescan_api_opendrive
import OpenScenarioImporter.python_interface.prescan.api.openscenario as prescan_api_openscenario
import PCS.python_interface.prescan.api.pcs as prescan_api_pcs
import PhysicsBasedCamera.python_interface.prescan.api.physicsbasedcameraunreal as prescan_api_physicsbasedcameraunreal
import PrescanViewer.python_interface.prescan.api.viewer as prescan_api_viewer
import Radar.python_interface.prescan.api.radar as prescan_api_radar
import TIS.python_interface.prescan.api.tis as prescan_api_tis
import TrafficSignalSensor.python_interface.prescan.api.trafficsignalsensor as prescan_api_trafficsignalsensor
import Trajectory.python_interface.prescan.api.scenario as prescan_api_scenario
import Trajectory.python_interface.prescan.api.trajectory as prescan_api_trajectory
import UltraSonic.python_interface.prescan.api.ultrasonic as prescan_api_ultrasonic
import UserData.python_interface.prescan.api.userdata as prescan_api_userdata
import VehicleDynamics.python_interface.prescan.api.vehicledynamics as prescan_api_vehicledynamics
import Algorithm.python_interface.prescan.internal.algorithm as prescan_internal_algorithm
import Trajectory.python_interface.prescan.internal.scenario as prescan_internal_scenario
import FullWaveformLidarPlugin.modules.fullwaveformlidar.python_interface.prescan.api.fullwaveformlidar as prescan_api_fullwaveformlidar
import PBRadarPlugin.modules.pbradar.python_interface.prescan.api.pbradar as prescan_api_pbradar
import PointCloudLidarPlugin.modules.pointcloudlidar.python_interface.prescan.api.pointcloudlidar as prescan_api_pointcloudlidar
import ProbabilisticSensorsPlugin.modules.ProbabilisticCamera.python_interface.prescan.api.probabilisticcamera as prescan_api_probabilisticcamera
import ProbabilisticSensorsPlugin.modules.ProbabilisticRadar.python_interface.prescan.api.probabilisticradar as prescan_api_probabilisticradar
import V2XPlugin.modules.v2x.python_interface.prescan.api.v2x as prescan_api_v2x
import GraphBasedRoads.python_interface.prescan.api.roads.types as prescan_api_roads_types
```

However, some users may not be able to access to all Prescan features. Then, user need to delete/commentout these API that you don't have access to. For example, if you don't have license for Physics-Based sensors, you need to delete the imported API for them. The following lists which API you need to delete or commentout if you don't have access to it.

if no access to v2x, delete/commmentout：

```python
#import prescan.api.v2x
#import V2XPlugin.modules.v2x.python_interface.prescan.api.v2x as prescan_api_v2x
```

if no access to physicsbasedcameraunreal, delete/commmentout：

```python
#import prescan.api.physicsbasedcameraunreal
#import PhysicsBasedCamera.python_interface.prescan.api.physicsbasedcameraunreal as prescan_api_physicsbasedcameraunreal
```

if no access to fullwaveformlidar, delete/commmentout：

```python
#import prescan.api.fullwaveformlidar
#import FullWaveformLidarPlugin.modules.fullwaveformlidar.python_interface.prescan.api.fullwaveformlidar as prescan_api_fullwaveformlidar
```

if no access to pbradar, delete/commmentout：

```python
#import prescan.api.pbradar
#import PBRadarPlugin.modules.pbradar.python_interface.prescan.api.pbradar as prescan_api_pbradar
```

if no access to pointcloudlidar, delete/commmentout：

```python
#import prescan.api.pointcloudlidar
#import PointCloudLidarPlugin.modules.pointcloudlidar.python_interface.prescan.api.pointcloudlidar as prescan_api_pointcloudlidar
```

if no access to probabilisticcamera/radar, delete/commmentout：

```python
#import prescan.api.probabilisticcamera
#import prescan.api.probabilisticradar
#import ProbabilisticSensorsPlugin.modules.ProbabilisticCamera.python_interface.prescan.api.probabilisticcamera as prescan_api_probabilisticcamera
#import ProbabilisticSensorsPlugin.modules.ProbabilisticRadar.python_interface.prescan.api.probabilisticradar as prescan_api_probabilisticradar
```
If you don't even find the Plugins folder in your Prescan installation path, you need to delete/commentput all above. In addition, Prescan doesn't support v2x on Linux. So you must delete/commentout v2x API import when you use the generator on Linux.


### sensors and generators
After you finish the delete/commentout of API import, you should continue to delete the relevant modules in  ```sensors``` and ```generator``` folders. For example, if you delete/commentout the API import of ```pointcloudlidar``` in  ```prescan_python_dmapi.py```, then you must **delete** ```Pointcloudlidar.py``` and ```PointcloudlidarGenerator.py``` in ```sensors``` and ```generator``` folders, respectively.

## Code Generation
==Before you start to generate simcpp codes, please follow the guide to delete/comment source codes according to the installation of Prescan features on your machine==.
SimCPP Generator can help generate the Prescan simcpp codes on both Windows and Ubuntu. Considering the flexibilty of Prescan Plugin installation on Ubuntu, plugin folders may have different path and names。Hence, it is recommended that you generate codes on windows and copy them to Ubuntu if you want to use some plugin features.

### Windows
1. First create or open a Prescan experiment. In this project, we have a demo experiment in ```./prescandemos```folder, called ```SimcppGenerator```.
2. In the Prescan GUI, you can create scenario, roads and actors，then set the simulation conditions, add trajectory or AmesimPreconfigedDynamics to actors，and finally add sensors to actors([supported sensors for auto generation](#introduction));
![GUI](./pic/GUI.png)
3. Modify the vars in ```set_env.bat```，e.g., when this project locates in ```C:\\Users\\yiyan5ez\\Desktop\\PrescanSimcppGenerator```, the following paths are Prescan experiment, pb file and simcpp:
    ```powershell
    set experiment_dir="C:\\Users\\yiyan5ez\\Desktop\\PrescanSimcppGenerator\\prescandemos\\SimcppGenerator"
    set experiment_pb="C:\\Users\\yiyan5ez\\Desktop\\PrescanSimcppGenerator\\prescandemos\\SimcppGenerator\\SimcppGenerator.pb"
    set simcpp_dir="C:\\Users\\yiyan5ez\\Desktop\\PrescanSimcppGenerator\\prescandemos\\SimcppGenerator\\simcpp"
    ```
4. Double click ```set_env.bat``` and a cmd Terminal will pop-up，type：
    ```powershell
    python main.py %experiment_pb% -out_dir %simcpp_dir%
    ```
    where, the first arguement ```%experiment_pb%``` is the path to pb file and the second optional argument ```-out_dir``` is where you want to put the generated simcpp codes, here we put it in ```%simcpp_dir%```. If ```%simcpp_dir%``` already exists，then there will be a warning and you need to confirm as shown in the following picture:
    ![Alt text](./pic/generate_win.png)
    input 'y' means to delete the existing simcpp folder and input 'n' or others mean to exit the program without doing anything. If codes are generated successfully, you will see the following outputs:
    ![Alt text](./pic/generated_win.png)
5. Go to the Prescan experiment folder, you can see a folder called 'simcpp' which includes all the C++ codes for this experiment simulation.
    ![Alt text](./pic/simcpp_win.png)
    Inside the folder，you can see the file strcture:
    ![Alt text](./pic/simmode_win.png)
    double click ```set_env.bat``` will pop up a cmd termina, type：
    ```powershell
    build.bat
    ```
    if build successfully，you will see the following outputs:
    ```powershell
    -- Selecting Windows SDK version 10.0.17763.0 to target Windows 10.0.22621.
    -- The CXX compiler identification is MSVC 19.20.27508.1
    -- Detecting CXX compiler ABI info
    -- Detecting CXX compiler ABI info - done
    -- Check for working CXX compiler: c:/apps/MVS16/VC/Tools/MSVC/14.20.27508/bin/Hostx64/x64/cl.exe - skipped
    -- Detecting CXX compile features
    -- Detecting CXX compile features - done
    -- Found Prescan 2023.1.0 at D:/Simcenter Prescan/Prescan_2302/lib/cmake
        Plugins found:
            - Prescan::FullWaveformLidar
            - Prescan::PBRadar
            - Prescan::PointCloudLidar
            - Prescan::ProbabilisticSensors
            - Prescan::v2x
    -- Generating helper scripts for demo: SimCppProject
    -- Experiment directory set to: C:/Users/yiyan5ez/Desktop/SimcppGenerator
    -- PrescanRootDIR D:/Simcenter Prescan/Prescan_2302
    -- Configuring done
    -- Generating done
    -- Build files have been written to: C:/Users/yiyan5ez/Desktop/SimcppGenerator/simcpp/build
    Microsoft (R) Build Engine version 16.0.462+g62fb89029d for .NET Framework
    Copyright (C) Microsoft Corporation. All rights reserved.

      Checking File Globs
      Checking Build System
      Building Custom Rule C:/Users/yiyan5ez/Desktop/SimcppGenerator/simcpp/CMakeLists.txt
      main.cpp
      cnpy.cpp
      Generating Code...
      SimCppProject.vcxproj -> C:\Users\yiyan5ez\Desktop\SimcppGenerator\simcpp\build\Release\SimCppProject.exe
      Building Custom Rule C:/Users/yiyan5ez/Desktop/SimcppGenerator/simcpp/CMakeLists.txt
    ```
    then double click ```set_env.bat``` to pop up another cmd window，type：
    ```powershell
    run.bat
    ```
    Prescan will start the simulation：
    ![Alt text](./pic/sim_win.png)
    if you want to change the simulation duration，open```run.bat```：
    ```powershell
    prescanrun %simcpp_dir%\\build\\Release\\SimCppProject.exe %experiment_pb% 10
    ```
    where，number 10 means simulation duration of 10 seconds.

### Ubuntu
1. Same as on Windows;
2. Same as on Windows;
3. Copy the Simcpp Generator project to Ubuntu;
4. Modify the vars in ```set_env.bash``` according to your experiment path:
    ```bash
    export experiment_dir=/home/yang/Desktop/PrescanSimcppGenerator/prescandemos/SimcppGenerator
    export experiment_pb=/home/yang/Desktop/PrescanSimcppGenerator/prescandemos/SimcppGenerator/SimcppGenerator.pb
    export simcpp_dir=/home/yang/Desktop/PrescanSimcppGenerator/prescandemos/SimcppGenerator/simcpp
    ```
    
5. Open a terminal from current SimCPP generator project folder，and type step by step：
    ```shell
    source set_env.bash
    ```
    ```shell
    python3 main.py $experiment_pb -out_dir $simcpp_dir
    ```
    if successful, you will find the 'simcpp' folder in your Prescan experiment.
    ![Alt text](./pic/simmodel_linux.png)
6. Open a terminal from simcpp folder，and type in sequence:
    ```shell
    source set_env.bash
    ```
    ```shell
    bash build.bash
    ```
    if build successfully, you will the following outputs:
    ```shell
    -- The CXX compiler identification is GNU 11.4.0
    -- Detecting CXX compiler ABI info
    -- Detecting CXX compiler ABI info - done
    -- Check for working CXX compiler: /usr/bin/c++ - skipped
    -- Detecting CXX compile features
    -- Detecting CXX compile features - done
    -- Found Prescan 2023.1.0 at /usr/local/Prescan_2302/lib/cmake
        Plugins found: <NONE>
    -- Generating helper scripts for demo: SimCppProject
    -- Experiment directory set to: /home/yang/Desktop/SimcppGenerator
    -- Configuring done
    -- Generating done
    -- Build files have been written to: /home/yang/Desktop/SimcppGenerator/simcpp/build
    [ 33%] Building CXX object CMakeFiles/SimCppProject.dir/simmodel/main.cpp.o
    In file included from /home/yang/Desktop/SimcppGenerator/simcpp/simmodel/simmodel/simmodel.h:21,
                    from /home/yang/Desktop/SimcppGenerator/simcpp/simmodel/main.cpp:23:
    /home/yang/Desktop/SimcppGenerator/simcpp/simmodel/utils/threadpool.h: In function ‘double prescan::utils::get_timestamp()’:
    /home/yang/Desktop/SimcppGenerator/simcpp/simmodel/utils/threadpool.h:56:8: warning: ‘int ftime(timeb*)’ is deprecated: Use gettimeofday or clock_gettime instead [-Wdeprecated-declarations]
      56 |   ftime(&now);
          |   ~~~~~^~~~~~
    In file included from /home/yang/Desktop/SimcppGenerator/simcpp/simmodel/utils/threadpool.h:38,
                    from /home/yang/Desktop/SimcppGenerator/simcpp/simmodel/simmodel/simmodel.h:21,
                    from /home/yang/Desktop/SimcppGenerator/simcpp/simmodel/main.cpp:23:
    /usr/include/x86_64-linux-gnu/sys/timeb.h:29:12: note: declared here
      29 | extern int ftime (struct timeb *__timebuf)
          |            ^~~~~
    [ 66%] Building CXX object CMakeFiles/SimCppProject.dir/simmodel/utils/cnpy.cpp.o
    [100%] Linking CXX executable SimCppProject
    /usr/bin/ld: warning: /usr/local/Prescan_2302/lib/libplatform.so has a program header with invalid alignment
    /usr/bin/ld: warning: /usr/local/Prescan_2302/lib/libpimp.so has a program header with invalid alignment
    /usr/bin/ld: warning: /usr/local/Prescan_2302/lib/libfederate_sdk.so has a program header with invalid alignment
    /usr/bin/ld: warning: /usr/local/Prescan_2302/lib/libpssettings.so has a program header with invalid alignment
    /usr/bin/ld: warning: /usr/local/Prescan_2302/lib/libmoduleloader.so has a program header with invalid alignment
    /usr/bin/ld: warning: /usr/local/Prescan_2302/lib/libprtl.so has a program header with invalid alignment
    /usr/bin/ld: warning: /usr/local/Prescan_2302/lib/libvislibrary.so has a program header with invalid alignment
    [100%] Built target SimCppProject
    ```
    then, continue with ```run.bash``` to execute the simulation. ==Please make sure Prescan DeploymentService is already runnning==。So, just open a terminal from simcpp and type:
    ```shell
    source set_env.bash
    ```
    ```shell
    bash run.bash
    ```
    the simulation is running as follows：
    ![Alt text](./pic/sim_linux.png)


# Details into Codes
## How to get sensor data


# Advanced Application
## SimCppBridge

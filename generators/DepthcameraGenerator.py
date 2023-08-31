#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.Depthcamera import Depthcamera
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generators.MetaGenerator import Generator, Include
from rename_api_namespace.prescan_python_dmapi import *
import shutil
from logger.UserLog import uniLog
from prescan.internal import PrescanException

__load_module__ = ["DepthcameraGenerator"]


class DepthcameraGenerator(Generator):
    sensorName = Sensor.Depthcamera

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_depthcamera_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[DepthcameraGenerator.sensorName]) > 0 and not self.include_depthcamera_dependency:
                self.includes += depthcamera_incl
                self.include_depthcamera_dependency = True
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_depthcamera.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_depthcamera.h",
                            self.dst + "/simmodel/sensorsdemux")

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        for depthCamera in _object.objectSensors[DepthcameraGenerator.sensorName]:  # type: Depthcamera
            depthCameraSensor_prefix = f"depthCameraSensor_{_object.ps_object.name}"
            depthCameraUnit_prefix = f"depthCameraUnit_{_object.ps_object.name}"
            if depthCamera.outputCameraImage:
                self.properties += f"{self.space4}//Add Depthcamera sensor properties\n"
                self.properties += f"{self.space4}{DepthcameraaSensor} {depthCameraSensor_prefix}_{depthCamera.camera.name};\n"
                self.properties += f"{self.space4}{DepthcameraSensorUnit}* {depthCameraUnit_prefix}_{depthCamera.camera.name}" + "{nullptr};\n"

                self.constructor += f"{self.space4}//Construct Depthcamera sensor properties\n"

                self.registerUnits += f"{self.space4}//register Depthcamera sensor units\n"
                self.registerUnits += f"{self.space4}{depthCameraSensor_prefix}_{depthCamera.camera.name} = " \
                                      f"{getDepthCameraSensor}(\"{depthCamera.camera.name}\");\n"
                self.registerUnits += f"{self.space4}{depthCameraUnit_prefix}_{depthCamera.camera.name} = " \
                                      f"{registerDepthCameraSensorUnit}(simulation, {depthCameraSensor_prefix}_{depthCamera.camera.name});\n"

                self.steps += f"{self.space4}//demux Depthcamera sensor outputs\n"

                # output properties and constructors
                self.properties += f"{self.space4}//D is output of DepthCamera, one channel with float values\n"
                self.properties += f"{self.space4}{float_vector_ptr} {depthCameraUnit_prefix}_{depthCamera.camera.name}_D;\n"
                self.constructor += f"{self.space4}{depthCameraUnit_prefix}_{depthCamera.camera.name}_D = {float_vector_ptr_make};\n"
                func_space = " " * 6
                port_D = f"{depthCameraUnit_prefix}_{depthCamera.camera.name}_D" \
                    if _object.enable_all_ports else Generator.Terminator
                self.steps += f"{self.space4}{sensorDemux}::demux_depthcamera(\n" \
                              f"{func_space}{depthCameraUnit_prefix}_{depthCamera.camera.name},\n" \
                              f"{func_space}//Demux:\n" \
                              f"{func_space}{port_D}); // -> D (valid)\n"

                self.constructor += "\n"
                self.registerUnits += "\n"
                self.properties += "\n"
                self.steps += "\n"

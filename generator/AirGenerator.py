#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.Air import Air
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generator.MetaGenerator import Generator, Include
import shutil

__load_module__ = ["AirGenerator"]


class AirGenerator(Generator):
    sensorName = Sensor.Air

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_air_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[AirGenerator.sensorName]) > 0 and not self.include_air_dependency:
                self.includes += air_incl
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_air.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_air.h", self.dst + "/simmodel/sensorsdemux")
                self.include_air_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        airSensor_prefix = f"airSensor_{_object.ps_object.name}"
        airUnit_prefix = f"airUnit_{_object.ps_object.name}"

        for air in _object.objectSensors[AirGenerator.sensorName]:  # type: Air
            self.properties += f"{self.space4}//Add Air sensor properties\n"
            self.properties += f"{self.space4}{AirSensor} {airSensor_prefix}_{air.air.name};\n"
            self.properties += f"{self.space4}{AirSensorUnit}* {airUnit_prefix}_{air.air.name}" + "{nullptr};\n"

            self.properties += f"{self.space4}{double_vector_ptr} {airUnit_prefix}_{air.air.name}_Range;\n"
            self.properties += f"{self.space4}{double_vector_ptr} {airUnit_prefix}_{air.air.name}_Theta;\n"
            self.properties += f"{self.space4}{double_vector_ptr} {airUnit_prefix}_{air.air.name}_Phi;\n"
            self.properties += f"{self.space4}{uint32_t_vector_ptr} {airUnit_prefix}_{air.air.name}_ID;\n"
            self.properties += f"{self.space4}{double_vector_ptr} {airUnit_prefix}_{air.air.name}_Velocity;\n"
            self.properties += f"{self.space4}{double_vector_ptr} {airUnit_prefix}_{air.air.name}_Heading;\n"
            self.properties += "\n"

            self.constructor += f"{self.space4}//Construct Air sensor properties\n"
            self.constructor += f"{self.space4}{airUnit_prefix}_{air.air.name}_Range = {double_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{airUnit_prefix}_{air.air.name}_Theta = {double_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{airUnit_prefix}_{air.air.name}_Phi = {double_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{airUnit_prefix}_{air.air.name}_ID = {uint32_t_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{airUnit_prefix}_{air.air.name}_Velocity = {double_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{airUnit_prefix}_{air.air.name}_Heading = {double_vector_ptr_make};\n"
            self.constructor += "\n"

            self.registerUnits += f"{self.space4}//register Air sensor units\n"
            self.registerUnits += f"{self.space4}{airSensor_prefix}_{air.air.name} = {getAirSensor}(\"{air.air.name}\");\n"
            self.registerUnits += f"{self.space4}{airUnit_prefix}_{air.air.name} = " \
                                  f"{registerAirSensorUnit}(simulation, {airSensor_prefix}_{air.air.name});\n"
            self.registerUnits += f"{self.space4}{airUnit_prefix}_{air.air.name}_Range->reserve({airSensor_prefix}_{air.air.name}.maxDetectableObjects());\n"
            self.registerUnits += f"{self.space4}{airUnit_prefix}_{air.air.name}_Theta->reserve({airSensor_prefix}_{air.air.name}.maxDetectableObjects());\n"
            self.registerUnits += f"{self.space4}{airUnit_prefix}_{air.air.name}_Phi->reserve({airSensor_prefix}_{air.air.name}.maxDetectableObjects());\n"
            self.registerUnits += f"{self.space4}{airUnit_prefix}_{air.air.name}_ID->reserve({airSensor_prefix}_{air.air.name}.maxDetectableObjects());\n"
            self.registerUnits += f"{self.space4}{airUnit_prefix}_{air.air.name}_Velocity->reserve({airSensor_prefix}_{air.air.name}.maxDetectableObjects());\n"
            self.registerUnits += f"{self.space4}{airUnit_prefix}_{air.air.name}_Heading->reserve({airSensor_prefix}_{air.air.name}.maxDetectableObjects());\n"
            self.registerUnits += "\n"

            self.steps += f"{self.space4}//demux air sensor outputs\n"
            func_len = len(f"{self.space4}{sensorDemux}::demux_air(")
            func_space = " " * 6
            self.steps += f"{self.space4}{sensorDemux}::demux_air(\n" \
                          f"{func_space}{airUnit_prefix}_{air.air.name},\n" \
                          f"{func_space}//Demux:\n" \
                          f"{func_space}Terminator, // ->Range (valid)\n" \
                          f"{func_space}Terminator, // ->Theta (valid)\n" \
                          f"{func_space}Terminator, // ->Phi (valid)\n" \
                          f"{func_space}Terminator, // ->ID (valid)\n" \
                          f"{func_space}Terminator, // ->Velocity (valid)\n" \
                          f"{func_space}Terminator); // ->Heading (valid)\n"
            self.steps += "\n"

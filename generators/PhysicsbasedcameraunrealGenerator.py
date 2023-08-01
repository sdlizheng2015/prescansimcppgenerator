#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.Physicsbasedcameraunreal import Physicsbasedcameraunreal
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generators.MetaGenerator import Generator, Include
import shutil

__load_module__ = ["PhysicsbasedcameraunrealGenerator"]


class PhysicsbasedcameraunrealGenerator(Generator):
    sensorName = Sensor.Physicsbasedcameraunreal

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_pbc_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[
                       PhysicsbasedcameraunrealGenerator.sensorName]) > 0 and not self.include_pbc_dependency:
                self.includes += pbc_incl
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_pbc.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_pbc.h", self.dst + "/simmodel/sensorsdemux")
                self.include_pbc_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        pbcSensor_prefix = f"pbcSensor_{_object.ps_object.name}"
        pbcUnit_prefix = f"pbcUnit_{_object.ps_object.name}"
        for pbc in _object.objectSensors[
            PhysicsbasedcameraunrealGenerator.sensorName]:  # type: Physicsbasedcameraunreal
            if pbc.outputImages:
                self.properties += f"{self.space4}//Add PBC sensor properties\n"
                self.properties += f"{self.space4}{PhysicsBasedCameraUnrealSensor} {pbcSensor_prefix}_{pbc.pbc.name};\n"
                self.properties += f"{self.space4}{PhysicsBasedCameraUnrealRGBUnit}* {pbcUnit_prefix}_{pbc.pbc.name}" + "{nullptr};\n"

                # output properties and constructors
                self.properties += f"{self.space4}{uint8_t_vector_ptr} {pbcUnit_prefix}_{pbc.pbc.name}_RGB8;\n"

                self.constructor += f"{self.space4}//add PBC property constructor\n"
                self.constructor += f"{self.space4}{pbcUnit_prefix}_{pbc.pbc.name}_RGB8 = {uint8_t_vector_ptr_make};\n"

                self.registerUnits += f"{self.space4}//register PBC sensor units\n"
                self.registerUnits += f"{self.space4}{pbcSensor_prefix}_{pbc.pbc.name} = {getPhysicsBasedCameraUnrealSensor}(\"{pbc.pbc.name}\");\n"
                self.registerUnits += f"{self.space4}{pbcUnit_prefix}_{pbc.pbc.name} = " \
                                      f"{registerPhysicsBasedCameraUnrealRGBUnit}(simulation, {pbcSensor_prefix}_{pbc.pbc.name});\n"
                func_space = " " * 6
                self.steps += f"{self.space4}//demux PBC sensor outputs\n"
                port_RGB8 = f"{pbcUnit_prefix}_{pbc.pbc.name}_RGB8" \
                    if _object.enable_all_ports else Generator.Terminator
                self.steps += f"{self.space4}{sensorDemux}::demux_pbc(\n" \
                              f"{func_space}{pbcUnit_prefix}_{pbc.pbc.name},\n" \
                              f"{func_space}//Demux:\n" \
                              f"{func_space}{port_RGB8}); // -> RGBU8 (valid)\n"

                self.steps += "\n"
                self.properties += "\n"
                self.registerUnits += "\n"
                self.constructor += "\n"

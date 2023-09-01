#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.Brs import Brs
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generators.MetaGenerator import Generator, Include
import shutil

__load_module__ = ["BrsGenerator"]


class BrsGenerator(Generator):
    sensorName = Sensor.Brs

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_brs_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[BrsGenerator.sensorName]) > 0 and not self.include_brs_dependency:
                self.includes += brs_incl
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_brs.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_brs.h", self.dst + "/simmodel/sensorsdemux")
                self.include_brs_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        brsSensor_prefix = f"brsSensor_{_object.ps_object.name}"
        brsUnit_prefix = f"brsUnit_{_object.ps_object.name}"

        for brs in _object.objectSensors[BrsGenerator.sensorName]:  # type: Brs
            self.properties += f"{self.space4}//Add Brs sensor properties\n"
            self.properties += f"{self.space4}{BrsSensor} {brsSensor_prefix}_{brs.brs.name};\n"
            self.properties += f"{self.space4}{BrsSensorUnit}* {brsUnit_prefix}_{brs.brs.name}" + "{nullptr};\n"

            # output properties
            self.properties += f"{self.space4}{uint32_t_vector_ptr} {brsUnit_prefix}_{brs.brs.name}_ObjectID;\n"
            self.properties += f"{self.space4}{double_vector_ptr} {brsUnit_prefix}_{brs.brs.name}_Left;\n"
            self.properties += f"{self.space4}{double_vector_ptr} {brsUnit_prefix}_{brs.brs.name}_Right;\n"
            self.properties += f"{self.space4}{double_vector_ptr} {brsUnit_prefix}_{brs.brs.name}_Bottom;\n"
            self.properties += f"{self.space4}{double_vector_ptr} {brsUnit_prefix}_{brs.brs.name}_Top;\n"

            self.constructor += f"{self.space4}//Construct Brs sensor properties\n"
            self.constructor += f"{self.space4}{brsUnit_prefix}_{brs.brs.name}_ObjectID = {uint32_t_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{brsUnit_prefix}_{brs.brs.name}_Left = {double_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{brsUnit_prefix}_{brs.brs.name}_Right = {double_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{brsUnit_prefix}_{brs.brs.name}_Bottom = {double_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{brsUnit_prefix}_{brs.brs.name}_Top = {double_vector_ptr_make};\n"

            self.registerUnits += f"{self.space4}//register Brs sensor units\n"
            self.registerUnits += f"{self.space4}{brsSensor_prefix}_{brs.brs.name} = " \
                                  f"{getBrsSensor}(\"{brs.brs.name}\");\n"
            self.registerUnits += f"{self.space4}{brsUnit_prefix}_{brs.brs.name} = " \
                                  f"{registerBrsSensorUnit}(simulation, {brsSensor_prefix}_{brs.brs.name});\n"

            self.steps += f"{self.space4}//demux brs outputs\n"
            func_space = " " * 6

            port_ObjectID = f"{brsUnit_prefix}_{brs.brs.name}_ObjectID" \
                if _object.enable_all_ports else Generator.Terminator
            port_Left = f"{brsUnit_prefix}_{brs.brs.name}_Left" \
                if _object.enable_all_ports else Generator.Terminator
            port_Right = f"{brsUnit_prefix}_{brs.brs.name}_Right" \
                if _object.enable_all_ports else Generator.Terminator
            port_Bottom = f"{brsUnit_prefix}_{brs.brs.name}_Bottom" \
                if _object.enable_all_ports else Generator.Terminator
            port_Top = f"{brsUnit_prefix}_{brs.brs.name}_Top" \
                if _object.enable_all_ports else Generator.Terminator

            self.steps += f"{self.space4}{sensorDemux}::demux_brs(\n" \
                          f"{func_space}{brsUnit_prefix}_{brs.brs.name},\n" \
                          f"{func_space}//Demux:\n" \
                          f"{func_space}{port_ObjectID}, // ->ObjectID (valid)\n" \
                          f"{func_space}{port_Left}, // ->Left (valid)\n" \
                          f"{func_space}{port_Right}, // ->Right (valid)\n" \
                          f"{func_space}{port_Bottom}, // ->Bottom (valid)\n" \
                          f"{func_space}{port_Top}); // ->Top (valid)\n"

            self.steps += "\n"
            self.properties += "\n"
            self.constructor += "\n"
            self.registerUnits += "\n"

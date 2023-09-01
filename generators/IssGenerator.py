#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.Iss import Iss
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generators.MetaGenerator import Generator, Include
import shutil

__load_module__ = ["IssGenerator"]


class IssGenerator(Generator):
    sensorName = Sensor.Iss

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_iss_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[IssGenerator.sensorName]) > 0 and not self.include_iss_dependency:
                self.includes += iss_incl
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_Iss.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_Iss.h", self.dst + "/simmodel/sensorsdemux")
                self.include_iss_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        issSensor_prefix = f"issSensor_{_object.ps_object.name}"
        issUnit_prefix = f"issUnit_{_object.ps_object.name}"

        for iss in _object.objectSensors[IssGenerator.sensorName]:  # type: Iss
            self.properties += f"{self.space4}//Add Iss sensor properties\n"
            self.properties += f"{self.space4}{IssSensor} {issSensor_prefix}_{iss.iss.name};\n"
            self.properties += f"{self.space4}{IssSensorUnit}* {issUnit_prefix}_{iss.iss.name}" + "{nullptr};\n"

            # output properties
            self.properties += f"{self.space4}{uint8_t_vector_ptr} {issUnit_prefix}_{iss.iss.name}_R;\n"
            self.properties += f"{self.space4}{uint8_t_vector_ptr} {issUnit_prefix}_{iss.iss.name}_G;\n"
            self.properties += f"{self.space4}{uint8_t_vector_ptr} {issUnit_prefix}_{iss.iss.name}_B;\n"

            self.constructor += f"{self.space4}//Construct Iss sensor properties\n"
            self.constructor += f"{self.space4}{issUnit_prefix}_{iss.iss.name}_R = {uint8_t_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{issUnit_prefix}_{iss.iss.name}_G = {uint8_t_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{issUnit_prefix}_{iss.iss.name}_B = {uint8_t_vector_ptr_make};\n"

            self.registerUnits += f"{self.space4}//register Iss sensor units\n"
            self.registerUnits += f"{self.space4}{issSensor_prefix}_{iss.iss.name} = " \
                                  f"{getIssSensor}(\"{iss.iss.name}\");\n"
            self.registerUnits += f"{self.space4}{issUnit_prefix}_{iss.iss.name} = " \
                                  f"{registerIssSensorUnit}(simulation, {issSensor_prefix}_{iss.iss.name});\n"

            self.steps += f"{self.space4}//demux Iss outputs\n"
            func_space = " " * 6

            port_R = f"{issUnit_prefix}_{iss.iss.name}_R" \
                if _object.enable_all_ports else Generator.Terminator
            port_G = f"{issUnit_prefix}_{iss.iss.name}_G" \
                if _object.enable_all_ports else Generator.Terminator
            port_B = f"{issUnit_prefix}_{iss.iss.name}_B" \
                if _object.enable_all_ports else Generator.Terminator

            self.steps += f"{self.space4}{sensorDemux}::demux_Iss(\n" \
                          f"{func_space}{issUnit_prefix}_{iss.iss.name},\n" \
                          f"{func_space}//Demux, RGB come in column:\n" \
                          f"{func_space}{port_R}, // ->R (valid)\n" \
                          f"{func_space}{port_G}, // ->G (valid)\n" \
                          f"{func_space}{port_B}); // ->B (valid)\n"

            self.steps += "\n"
            self.properties += "\n"
            self.constructor += "\n"
            self.registerUnits += "\n"

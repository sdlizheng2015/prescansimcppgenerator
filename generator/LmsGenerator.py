#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.Lms import Lms
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generator.MetaGenerator import Generator, Include
import shutil

__load_module__ = ["LmsGenerator"]


class LmsGenerator(Generator):
    sensorName = Sensor.Lms

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_lms_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[LmsGenerator.sensorName]) > 0 and not self.include_lms_dependency:
                self.includes += lms_incl
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_lms.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_lms.h", self.dst + "/simmodel/sensorsdemux")
                self.include_lms_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        lmsSensor_prefix = f"lmsSensor_{_object.ps_object.name}"
        lmsUnit_prefix = f"lmsUnit_{_object.ps_object.name}"

        for lms in _object.objectSensors[LmsGenerator.sensorName]:  # type: Lms
            self.properties += f"{self.space4}//Add Lms sensor properties\n"
            self.properties += f"{self.space4}{LmsSensor} {lmsSensor_prefix}_{lms.lms.name};\n"
            self.properties += f"{self.space4}{LmsSensorUnit}* {lmsUnit_prefix}_{lms.lms.name}" + "{nullptr};\n"

            # output properties
            self.properties += f"{self.space4}{lms_line_vector_ptr} {lmsUnit_prefix}_{lms.lms.name}_Lines;\n"

            self.constructor += f"{self.space4}//Construct Lms sensor properties\n"
            self.constructor += f"{self.space4}{lmsUnit_prefix}_{lms.lms.name}_Lines = {lms_line_vector_ptr_make};\n"

            self.registerUnits += f"{self.space4}//register Lms sensor units\n"
            self.registerUnits += f"{self.space4}{lmsSensor_prefix}_{lms.lms.name} = " \
                                  f"{getLmsSensor}(\"{lms.lms.name}\");\n"
            self.registerUnits += f"{self.space4}{lmsUnit_prefix}_{lms.lms.name} = " \
                                  f"{registerLmsSensorUnit}(simulation, {lmsSensor_prefix}_{lms.lms.name}, " \
                                  f"simulation->getSimulationPath());\n"

            self.steps += f"{self.space4}//demux lms sensor outputs\n"
            func_space = " " * 6

            port_Lines = f"{lmsUnit_prefix}_{lms.lms.name}_Lines" \
                if _object.enable_all_ports else Generator.Terminator

            self.steps += f"{self.space4}{sensorDemux}::demux_lms(\n" \
                          f"{func_space}{lmsUnit_prefix}_{lms.lms.name},\n" \
                          f"{func_space}//Demux:\n" \
                          f"{func_space}{port_Lines}); // ->Lines (valid)\n"

            self.steps += "\n"
            self.properties += "\n"
            self.constructor += "\n"
            self.registerUnits += "\n"

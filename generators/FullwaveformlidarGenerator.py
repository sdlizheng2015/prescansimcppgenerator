#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

IS_PLUGIN = True

from sensors.Fullwaveformlidar import Fullwaveformlidar
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generators.MetaGenerator import Generator, Include
import shutil

__load_module__ = ["FullwaveformlidarGenerator"]


class FullwaveformlidarGenerator(Generator):
    sensorName = Sensor.Fullwaveformlidar

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_fwl_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[
                       FullwaveformlidarGenerator.sensorName]) > 0 and not self.include_fwl_dependency:
                self.includes += fwl_incl
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_fwl.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_fwl.h", self.dst + "/simmodel/sensorsdemux")
                self.include_fwl_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        fwlSensor_prefix = f"fwlSensor_{_object.ps_object.name}"
        fwlPointUnit_prefix = f"fwlPointUnit_{_object.ps_object.name}"
        fwlBeamUnit_prefix = f"fwlBeamUnit_{_object.ps_object.name}"
        fwlInfoUnit_prefix = f"fwlInfoUnit_{_object.ps_object.name}"
        fwlPowerUnit_prefix = f"fwlPowerUnit_{_object.ps_object.name}"
        for fwl in _object.objectSensors[FullwaveformlidarGenerator.sensorName]:  # type: Fullwaveformlidar
            self.properties += f"{self.space4}//Add FWL sensor properties\n"
            self.properties += f"{self.space4}{FullWaveformLidarSensor} {fwlSensor_prefix}_{fwl.fwl.name};\n"
            self.properties += f"{self.space4}{FwlPointCloudUnit}* {fwlPointUnit_prefix}_{fwl.fwl.name}" + "{nullptr};\n"
            self.properties += f"{self.space4}{FwlBeamUnit}* {fwlBeamUnit_prefix}_{fwl.fwl.name}" + "{nullptr};\n"
            self.properties += f"{self.space4}{FwlOutputInfoUnit}* {fwlInfoUnit_prefix}_{fwl.fwl.name}" + "{nullptr};\n"
            if fwl.enableEnergyCalculation:
                self.properties += f"{self.space4}{FwlPowerUnit}* {fwlPowerUnit_prefix}_{fwl.fwl.name}" + "{nullptr};\n"

            # output properties
            self.properties += f"{self.space4}{float_vector_ptr} {fwlPointUnit_prefix}_{fwl.fwl.name}_X;\n"
            self.properties += f"{self.space4}{float_vector_ptr} {fwlPointUnit_prefix}_{fwl.fwl.name}_Y;\n"
            self.properties += f"{self.space4}{float_vector_ptr} {fwlPointUnit_prefix}_{fwl.fwl.name}_Z;\n"
            if fwl.enableEnergyCalculation:
                self.properties += f"{self.space4}{float_vector_ptr} {fwlPowerUnit_prefix}_{fwl.fwl.name}_I;\n"
            self.properties += f"{self.space4}{fwl_beam_vector_ptr} {fwlBeamUnit_prefix}_{fwl.fwl.name}_Beam;\n"
            self.properties += f"{self.space4}{fwl_info_vector_ptr} {fwlInfoUnit_prefix}_{fwl.fwl.name}_Info;\n"

            # output constructors
            self.constructor += f"{self.space4}//Construct FWL sensor properties\n"
            self.constructor += f"{self.space4}{fwlPointUnit_prefix}_{fwl.fwl.name}_X = {float_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{fwlPointUnit_prefix}_{fwl.fwl.name}_Y = {float_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{fwlPointUnit_prefix}_{fwl.fwl.name}_Z = {float_vector_ptr_make};\n"
            if fwl.enableEnergyCalculation:
                self.constructor += f"{self.space4}{fwlPowerUnit_prefix}_{fwl.fwl.name}_I = {float_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{fwlBeamUnit_prefix}_{fwl.fwl.name}_Beam = {fwl_beam_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{fwlInfoUnit_prefix}_{fwl.fwl.name}_Info = {fwl_info_vector_ptr_make};\n"

            self.registerUnits += f"{self.space4}//register FWL sensor units\n"
            self.registerUnits += f"{self.space4}{fwlSensor_prefix}_{fwl.fwl.name} = {getFullWaveformLidarSensor}(\"{fwl.fwl.name}\");\n"
            self.registerUnits += f"{self.space4}{fwlPointUnit_prefix}_{fwl.fwl.name} = " \
                                  f"{registerFwlPointCloudUnit}(simulation, {fwlSensor_prefix}_{fwl.fwl.name});\n"
            self.registerUnits += f"{self.space4}{fwlBeamUnit_prefix}_{fwl.fwl.name} = " \
                                  f"{registerFwlBeamUnit}(simulation, {fwlSensor_prefix}_{fwl.fwl.name});\n"
            self.registerUnits += f"{self.space4}{fwlInfoUnit_prefix}_{fwl.fwl.name} = " \
                                  f"{registerFwlOutputInfoUnit}(simulation, {fwlSensor_prefix}_{fwl.fwl.name});\n"

            if fwl.enableEnergyCalculation:
                self.registerUnits += f"{self.space4}{fwlPowerUnit_prefix}_{fwl.fwl.name} = " \
                                      f"{registerFwlPowerUnit}(simulation, {fwlSensor_prefix}_{fwl.fwl.name});\n"

            self.steps += f"{self.space4}//demux FWL sensor outputs\n"
            func_space = " " * 6
            port_X = f"{fwlPointUnit_prefix}_{fwl.fwl.name}_X" \
                if _object.enable_all_ports else Generator.Terminator
            port_Y = f"{fwlPointUnit_prefix}_{fwl.fwl.name}_Y" \
                if _object.enable_all_ports else Generator.Terminator
            port_Z = f"{fwlPointUnit_prefix}_{fwl.fwl.name}_Z" \
                if _object.enable_all_ports else Generator.Terminator
            port_I = f"{fwlPowerUnit_prefix}_{fwl.fwl.name}_I" \
                if (_object.enable_all_ports and fwl.enableEnergyCalculation) else Generator.Terminator
            port_Beam = f"{fwlBeamUnit_prefix}_{fwl.fwl.name}_Beam" \
                if _object.enable_all_ports else Generator.Terminator
            port_Info = f"{fwlInfoUnit_prefix}_{fwl.fwl.name}_Info" \
                if _object.enable_all_ports else Generator.Terminator

            powerUnit = f"{fwlPowerUnit_prefix}_{fwl.fwl.name}" if fwl.enableEnergyCalculation else Generator.Terminator

            self.steps += f"{self.space4}{sensorDemux}::demux_fwl(\n" \
                          f"{func_space}simmodel,\n" \
                          f"{func_space}{fwlSensor_prefix}_{fwl.fwl.name},\n" \
                          f"{func_space}{fwlPointUnit_prefix}_{fwl.fwl.name},\n" \
                          f"{func_space}{fwlInfoUnit_prefix}_{fwl.fwl.name},\n" \
                          f"{func_space}{fwlBeamUnit_prefix}_{fwl.fwl.name},\n" \
                          f"{func_space}{powerUnit},\n" \
                          f"{func_space}//Demux:\n" \
                          f"{func_space}{port_X}, // ->X (valid)\n" \
                          f"{func_space}{port_Y}, // ->Y (valid)\n" \
                          f"{func_space}{port_Z}, // ->Z (valid)\n" \
                          f"{func_space}{port_I}, // ->I (valid)\n" \
                          f"{func_space}{port_Beam}, // ->Beam (valid)\n" \
                          f"{func_space}{port_Info}); // ->Info (valid)\n"

            self.constructor += "\n"
            self.steps += "\n"
            self.properties += "\n"
            self.registerUnits += "\n"

#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.Pcs import Pcs
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generator.MetaGenerator import Generator, Include
import shutil
from logger.UserLog import uniLog
from prescan.internal import PrescanException

__load_module__ = ["PCSGenerator"]


class PCSGenerator(Generator):
    sensorName = Sensor.Pcs

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_pcs_position = False
            self.include_pcs_rang = False
            self.include_pcs_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[PCSGenerator.sensorName]) > 0 and not self.include_pcs_dependency:
                self.includes += pcs_incl
                self.include_pcs_dependency = True

            if not (self.include_pcs_rang and self.include_pcs_position):
                for _pcs in _object.objectSensors[PCSGenerator.sensorName]:  # type: Pcs
                    if _pcs.outputMode == Pcs.OutputMode.Range:
                        self.sensorDemux += f'''{sensorDemux_incl_prefix}_pcsRange.h"\n'''
                        shutil.copy(f"{sensorDemux_file_prefix}_pcsRange.h", self.dst + "/simmodel/sensorsdemux")
                        self.include_pcs_rang = True
                    elif _pcs.outputMode in [Pcs.OutputMode.SensorPosition, Pcs.OutputMode.WorldPosition]:
                        self.sensorDemux += f'''{sensorDemux_incl_prefix}_pcsPosition.h"\n'''
                        shutil.copy(f"{sensorDemux_file_prefix}_pcsPosition.h", self.dst + "/simmodel/sensorsdemux")
                        self.include_pcs_position = True
                    else:
                        uniLog.logger.error("Unsupported pcs format for #inlcude")
                        raise PrescanException("Unsupported pcs format for #inlcude")

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        pcsSensor_prefix = f"pcsSensor_{_object.ps_object.name}"
        pcsUnit_prefix = f"pcsUnit_{_object.ps_object.name}"
        for pcs in _object.objectSensors[PCSGenerator.sensorName]:  # type: Pcs
            self.properties += f"{self.space4}//Add PCS sensor properties\n"
            self.properties += f"{self.space4}{PcsSensor} {pcsSensor_prefix}_{pcs.pcs.name};\n"
            self.properties += f"{self.space4}{PcsSensorUnit}* {pcsUnit_prefix}_{pcs.pcs.name}" + "{nullptr};\n"

            self.registerUnits += f"{self.space4}//register PCS sensor units\n"
            self.registerUnits += f"{self.space4}{pcsSensor_prefix}_{pcs.pcs.name} = {getPcsSensor}(\"{pcs.pcs.name}\");\n"
            self.registerUnits += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name} = " \
                                  f"{registerPcsSensorUnit}(simulation, {pcsSensor_prefix}_{pcs.pcs.name});\n"

            self.steps += f"{self.space4}//demux PCS sensor outputs\n"
            self.constructor += f"{self.space4}//Add PCS constructors\n"
            if pcs.outputMode == Pcs.OutputMode.Range:
                # output properties and constructors
                self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_Range;\n"
                self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_Range = {float_vector_ptr_make};\n"
                if pcs.outputDopplerVelocity:
                    self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_Velocity;\n"
                    self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_Velocity = {float_vector_ptr_make};\n"
                if pcs.outputIntensity:
                    self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_Intensity;\n"
                    self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_Intensity = {float_vector_ptr_make};\n"
                if pcs.outputObjectID:
                    self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_ID;\n"
                    self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_ID = {float_vector_ptr_make};\n"
                if pcs.outputAngles:
                    self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_Angle_Theta;\n"
                    self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_Angle_Phi;\n"
                    self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_Angle_Theta = {float_vector_ptr_make};\n"
                    self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_Angle_Phi = {float_vector_ptr_make};\n"
                func_space = " " * 6
                port_Range = f"{pcsUnit_prefix}_{pcs.pcs.name}_Range" \
                    if _object.enable_all_ports else Generator.Terminator
                port_Velocity = f"{pcsUnit_prefix}_{pcs.pcs.name}_Velocity" \
                    if (_object.enable_all_ports and pcs.outputDopplerVelocity) else Generator.Terminator
                port_Intensity = f"{pcsUnit_prefix}_{pcs.pcs.name}_Intensity" \
                    if (_object.enable_all_ports and pcs.outputIntensity) else Generator.Terminator
                port_ID = f"{pcsUnit_prefix}_{pcs.pcs.name}_ID" \
                    if (_object.enable_all_ports and pcs.outputObjectID) else Generator.Terminator
                port_Angle_Theta = f"{pcsUnit_prefix}_{pcs.pcs.name}_Angle_Theta" \
                    if (_object.enable_all_ports and pcs.outputAngles) else Generator.Terminator
                port_Angle_Phi = f"{pcsUnit_prefix}_{pcs.pcs.name}_Angle_Phi" \
                    if (_object.enable_all_ports and pcs.outputAngles) else Generator.Terminator

                self.steps += f"{self.space4}//If the output is disabled, DON'T/CAN'T replace its Terminator\n"
                self.steps += f"{self.space4}{sensorDemux}::demux_pcsRange(\n" \
                              f"{func_space}{pcsUnit_prefix}_{pcs.pcs.name},\n" \
                              f"{func_space}{pcsSensor_prefix}_{pcs.pcs.name},\n" \
                              f"{func_space}{int(pcs.outputDopplerVelocity)}, // outputDopplerVelocity?, DON'T EDIT\n" \
                              f"{func_space}{int(pcs.outputIntensity)}, // outputIntensity?, DON'T EDIT\n" \
                              f"{func_space}{int(pcs.outputObjectID)}, // outputObjectID?, DON'T EDIT\n" \
                              f"{func_space}{int(pcs.outputAngles)}, // outputAngles?, DON'T EDIT\n" \
                              f"{func_space}//Demux:\n" \
                              f"{func_space}{port_Range}, // ->Range (valid)\n" \
                              f"{func_space}{port_Velocity}, // ->Velocity {'(valid)' if pcs.outputDopplerVelocity else '(invalid)'}\n" \
                              f"{func_space}{port_Intensity}, // ->Intensity {'(valid)' if pcs.outputIntensity else '(invalid)'}\n" \
                              f"{func_space}{port_ID}, // ->ID {'(valid)' if pcs.outputObjectID else '(invalid)'}\n" \
                              f"{func_space}{port_Angle_Theta}, // ->Angle_Theta {'(valid)' if pcs.outputAngles else '(invalid)'}\n" \
                              f"{func_space}{port_Angle_Phi}); // ->Angle_Phi {'(valid)' if pcs.outputAngles else '(invalid)'}\n"
            else:
                # output properties and constructors
                self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_X;\n"
                self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_Y;\n"
                self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_Z;\n"
                self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_X = {float_vector_ptr_make};\n"
                self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_Y = {float_vector_ptr_make};\n"
                self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_Z = {float_vector_ptr_make};\n"
                if pcs.outputObjectID:
                    self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_ID;\n"
                    self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_ID = {float_vector_ptr_make};\n"
                if pcs.outputIntensity:
                    self.properties += f"{self.space4}{float_vector_ptr} {pcsUnit_prefix}_{pcs.pcs.name}_I;\n"
                    self.constructor += f"{self.space4}{pcsUnit_prefix}_{pcs.pcs.name}_I = {float_vector_ptr_make};\n"
                func_space = " " * 6
                port_X = f"{pcsUnit_prefix}_{pcs.pcs.name}_X" \
                    if _object.enable_all_ports else Generator.Terminator
                port_Y = f"{pcsUnit_prefix}_{pcs.pcs.name}_Y" \
                    if _object.enable_all_ports else Generator.Terminator
                port_Z = f"{pcsUnit_prefix}_{pcs.pcs.name}_Z" \
                    if _object.enable_all_ports else Generator.Terminator
                port_ID = f"{pcsUnit_prefix}_{pcs.pcs.name}_ID" \
                    if (_object.enable_all_ports and pcs.outputObjectID) else Generator.Terminator
                port_I = f"{pcsUnit_prefix}_{pcs.pcs.name}_I" \
                    if (_object.enable_all_ports and pcs.outputIntensity) else Generator.Terminator

                self.steps += f"{self.space4}//If the output is disabled, DON'T/CAN'T replace its Terminator\n"
                self.steps += f"{self.space4}{sensorDemux}::demux_pcsPosition(\n" \
                              f"{func_space}{pcsUnit_prefix}_{pcs.pcs.name},\n" \
                              f"{func_space}{pcsSensor_prefix}_{pcs.pcs.name},\n" \
                              f"{func_space}{int(pcs.outputIntensity)}, // outputIntensity?, DON'T EDIT\n" \
                              f"{func_space}{int(pcs.outputObjectID)}, // outputObjectID?, DON'T EDIT\n" \
                              f"{func_space}//Demux:\n" \
                              f"{func_space}{port_X}, // ->X (valid)\n" \
                              f"{func_space}{port_Y}, // ->Y (valid)\n" \
                              f"{func_space}{port_Z}, // ->Z (valid)\n" \
                              f"{func_space}{port_I}, // ->I {'(valid)' if pcs.outputIntensity else '(invalid)'}\n" \
                              f"{func_space}{port_ID}); // ->ID {'(valid)' if pcs.outputObjectID else '(invalid)'}\n"
            self.steps += "\n"
            self.properties += "\n"
            self.registerUnits += "\n"
            self.constructor += "\n"

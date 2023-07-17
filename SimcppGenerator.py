#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

import shutil
import os
import sys

from generator import *
from typing import Dict, List, Tuple
from sensors.MetaSensor import Sensor
from ObjectParser import ObjectParser, GeneratorObject
from utils.load_modules import get_cls
from generator import SelfUnitGenerator, StateActuatorGenerator


class SimcppGenerator:
    def __init__(self, pb: str, pb_yaml: str, ps_dir: str, load_yaml: bool = True):
        self.objectParser = ObjectParser(pb, pb_yaml, load_yaml)
        self.ps_dir = ps_dir
        self.dst = ""
        self.includes = ""
        self.properties = ""
        self.registerUnits = ""
        self.updates = ""
        self.steps = ""
        self.terminate = ""
        self.constructor = ""
        self.initialize = ""

    def copy_to_project(self, dst: str = "./projects"):
        """
        TODO: call this first
        :param dst:
        :return:
        """
        self.dst = dst
        if os.path.exists(dst):
            enter = input("Deleting the exiting project?(y/n):\n")
            if enter in ["y", "Y", "yes"]:
                try:
                    os.rmdir(dst)
                except OSError:
                    shutil.rmtree(dst)
                except Exception as ee:
                    print(ee)
                finally:
                    shutil.copytree("./templates/simcpp", dst)
                    print("deleting the exiting project and copy templates to project")
            else:
                print("not copy templates to project")
                sys.exit()
        else:
            shutil.copytree("./templates/simcpp", dst)
            print("copy templates to project")

    @staticmethod
    def overwrite_files(files: Dict[str, Dict[str, str]], new_path: str = ""):
        """
        TODO:
        :param files:
        :param new_path: if has, this function should work with only one file
        :return:
        """
        for path, flags_contents in files.items():
            with open(path, "r") as file:
                text = file.readlines()
                for flag, content in flags_contents.items():
                    text[text.index(flag)] = content
            write_to_path = new_path if new_path else path
            with open(write_to_path, "w") as file:
                file.writelines(text)

    def add_comments(self, _object: GeneratorObject):
        propertyComment = f"{Generator.space2}//Define properties for {_object.ps_object.name}\n"
        self.properties += propertyComment

        updatesComment = f"{Generator.space2}//Update states for {_object.ps_object.name}\n"
        self.updates += updatesComment

        registerUnitsComment = f"{Generator.space2}//Register units for {_object.ps_object.name}\n"
        self.registerUnits += registerUnitsComment

        stepsComment = f"{Generator.space2}//Demux sensors for {_object.ps_object.name}\n"
        self.steps += stepsComment

    def generate_codes(self):
        generator_names = [os.path.splitext(module)[0] for module in os.listdir("./generator") if
                           os.path.splitext(module)[1] == ".py"]
        generator_cls, incl_cls = get_cls(generator_names, "generator", sub_cls_name="SensorInclude")
        generator_cls.insert(0, generator_cls.pop(generator_cls.index(StateActuatorGenerator)))
        generator_cls.insert(0, generator_cls.pop(generator_cls.index(SelfUnitGenerator)))
        incl_cls.insert(0, incl_cls.pop(incl_cls.index(StateActuatorGenerator.SensorInclude)))
        incl_cls.insert(0, incl_cls.pop(incl_cls.index(SelfUnitGenerator.SensorInclude)))

        for _object in self.objectParser.ParsedObjects:  # type: GeneratorObject
            sensor_demux = ""
            sensor_includes = ""
            properties = ""
            registerUnits = ""
            updates = ""
            steps = ""
            terminate = ""
            constructor = ""
            initialize = ""
            worldObject = _object.ps_object.name
            className = f"class {worldObject}" + "{" + f"\npublic:\n  explicit {worldObject}()" + "{\n"
            deconstructfunc = f"  ~{worldObject}()" + "{\n"

            includes: List[Include] = [incl(self.dst) for incl in incl_cls]
            for include in includes:
                include.generate_codes(_object)
                sensor_demux += include.sensorDemux
                sensor_includes += include.includes

            generators: Dict[str, Generator] = {generator.sensorName: generator() for generator in generator_cls}

            for generator in generators.values():
                generator.generate_codes(_object)
                properties += generator.properties
                registerUnits += generator.registerUnits
                steps += generator.steps
                constructor += generator.constructor

            """update amesim if has amesim else update trajectory else nothing"""
            if generators[Sensor.AmesimDynamics].has_amesim:
                updates += generators[Sensor.AmesimDynamics].updates + "\n"
            elif generators[Sensor.Trajectory].has_trajectory:
                updates += generators[Sensor.Trajectory].updates + "\n"
            else:
                pass

            object_model_path = self.dst + "/simmodel/worldobjects/worldobject.h"
            object_model = {
                "//INCLUDES//\n": sensor_includes,
                "//SENSORMAP//\n": sensor_demux,
                "//ClASSNAME//\n": className,
                "//CONSTRUCT//\n": constructor,
                "//REGISTER//\n": registerUnits,
                "//UPDATE//\n": updates,
                "//STEP//\n": steps,
                "//PROPERTIES//\n": properties,
                "//DECONSTRUCTFUNC//\n": deconstructfunc,
            }
            files = {
                object_model_path: object_model,
            }
            self.overwrite_files(files, new_path=self.dst + f"/simmodel/worldobjects/{worldObject}.h")

            self.add_comments(_object)
            self.includes += f'''#include "worldobjects/{worldObject}.h"\n'''
            self.properties += f"{Generator.space2}{worldObject} ps_{worldObject};\n"
            self.registerUnits += f"{Generator.space2}ps_{worldObject}.registerSimulationUnits(experiment, simulation);\n"
            self.updates += f"{Generator.space2}ps_{worldObject}.updateState();\n"
            self.steps += f"{Generator.space2}ps_{worldObject}.step(simulation, this);\n"

    def write_to_project(self):
        simmodel_h_path = self.dst + "/simmodel/simmodel/simmodel.h"
        simmodel_h = {
            "//INCLUDES//\n": self.includes,
            "//PROPERTIES//\n": self.properties,
            "//REGISTER//\n": self.registerUnits,
            "//UPDATE//\n": self.updates,
            "//STEP//\n": self.steps,
        }

        cmake_list_path = self.dst + "/CMakeLists.txt"
        cmake_list = {
            "#PRESCAN_DIR#\n": f'''set(Prescan_DIR "{self.ps_dir}")\n''',
        }

        files = {
            simmodel_h_path: simmodel_h,
            cmake_list_path: cmake_list,
        }

        self.overwrite_files(files)

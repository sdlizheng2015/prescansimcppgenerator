#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 10/11/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["PathFollower"]


class DriverSensor:
    def __init__(self, ps_object: prescan_api_types.WorldObject):
        self.name = ps_object.name


class PathFollower(Sensor):
    name = Sensor.PathFollower
    getSensorsAPI = lambda ps_object: [DriverSensor(ps_object)]
    pimp_model = "pimp/userworld"

    def __init__(self, driver: DriverSensor, xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.driver = driver
        self.parse()

    def parse(self):
        pass

    def __str__(self):
        return f"    {self.driver.name}\n"

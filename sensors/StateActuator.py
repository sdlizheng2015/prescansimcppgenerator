#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["StateActuator"]


class StateActuator(Sensor):
    name = Sensor.StateActuator
    getSensorsAPI = lambda ps_object: []
    pimp_model = ""

    def __init__(self, obj: prescan_api_types.WorldObject, xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.obj = obj
        self.parse()

    def parse(self):
        pass

    def __str__(self):
        return f"    {self.obj.name}\n"

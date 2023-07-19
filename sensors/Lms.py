#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["Lms"]


class Lms(Sensor):
    name = Sensor.Lms
    getSensorsAPI = prescan_api_lms.getAttachedLmsSensors
    pimp_model = "pimp/lmsmodel"

    def __init__(self, lms: prescan_api_lms.LmsSensor, xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.lms = lms
        self.parse()

    def parse(self):
        pass

    def __str__(self):
        return f"    {self.lms.name}\n"

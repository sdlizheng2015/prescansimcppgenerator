#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from abc import abstractmethod, ABCMeta
from ObjectsSensorsParser import ObjectSensors

__load_module__ = []


class Generator(metaclass=ABCMeta):
    space2 = "    "

    def __init__(self):
        self.space4 = "    "

        self.properties = ""
        self.registerUnits = ""
        self.steps = ""
        self.updates = ""
        self.terminate = ""
        self.constructor = ""
        self.initialize = ""
        self.has_amesim = False
        self.has_trajectory = False

    @abstractmethod
    def generate_codes(self, _object: ObjectSensors):
        ...


class Include(metaclass=ABCMeta):
    def __init__(self, dst: str):
        self.dst = dst
        self.includes = ""
        self.sensorDemux = ""

    @abstractmethod
    def generate_codes(self, _object: ObjectSensors):
        ...

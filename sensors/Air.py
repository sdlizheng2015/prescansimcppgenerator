from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["Air"]


class Air(Sensor):
    name = Sensor.Air
    getSensorsAPI = prescan_api_air.getAttachedAirSensors
    pimp_model = "pimp/airsensormodel"

    def __init__(self, air: prescan_api_air.AirSensor, xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.air = air
        self.parse()

    def parse(self):
        pass

    def __str__(self):
        return f"    {self.air.name}\n"

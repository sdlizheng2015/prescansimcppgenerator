from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["Fullwaveformlidar"]


class Fullwaveformlidar(Sensor):
    name = Sensor.Fullwaveformlidar
    getSensorsAPI = prescan_api_fullwaveformlidar.getAttachedFullWaveformLidarSensors
    pimp_model = "fullwaveformlidarmodel"

    def __init__(self, fwl: prescan_api_fullwaveformlidar.FullWaveformLidarSensor,
                 xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.fwl = fwl
        self.enableEnergyCalculation = False
        self.parse()

    def parse(self):
        self.enableEnergyCalculation = self.fwl.powerCalculation

    def __str__(self):
        return f"    {self.fwl.name}: \n" \
               f"      enableEnergyCalculation: {self.enableEnergyCalculation}\n"

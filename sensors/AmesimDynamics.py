from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["AmesimDynamics"]


class AmesimDynamics(Sensor):
    name = Sensor.AmesimDynamics
    getSensorsAPI = lambda ps_object: [prescan_api_vehicledynamics.getAttachedAmesimPreconfiguredDynamics(ps_object)]
    pimp_model = "pimp/vehicledynamicsmodel"

    def __init__(self, amesim: prescan_api_vehicledynamics.AmesimPreconfiguredDynamics,
                 xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.amesim = amesim
        self.flatGround = False
        self.parse()

    def parse(self):
        self.flatGround = self.amesim.flatGround

    def __str__(self):
        return f"    Amesim_category: {self.amesim.category}: \n" \
               f"      flatGround: {self.flatGround}\n"

from ObjectParser import GeneratorObject
from sensors.MetaSensor import Sensor
from rename_api_namespace.simcpp_api import *
from generator.MetaGenerator import Generator, Include

__load_module__ = ["StateActuatorGenerator"]


class StateActuatorGenerator(Generator):
    sensorName = Sensor.StateActuator

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_state_actuator_dependency = False

        def generate_codes(self, _object: GeneratorObject):
            if not self.include_state_actuator_dependency:
                self.includes += state_incl
                self.include_state_actuator_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: GeneratorObject):
        stateUnit_prefix = f"stateUnit_{_object.ps_object.name}"
        object_var = f"obj_{_object.ps_object.name}"

        self.properties += f"{self.space4}//Add StateActuator properties\n"
        self.properties += f"{self.space4}{StateActuatorUnit}* {stateUnit_prefix}" + "{nullptr};\n"
        self.properties += "\n"

        self.registerUnits += f"{self.space4}//register StateActuator units\n"
        self.registerUnits += f"{self.space4}{stateUnit_prefix} = {registerStateActuatorUnit}(simulation, {object_var});\n"
        self.registerUnits += "\n"

#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 8/22/2023 11:59 AM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com
import os
import platform
from typing import List, Dict

comment = '''__load_module__ = []\n'''


class _Plugins:
    FullWaveformLidar = "FullWaveformLidar"
    PBRadar = "PBRadar"
    PointCloudLidar = "PointCloudLidar"
    Probabilistic = "Probabilistic"
    PhysicsBasedCamera = "PhysicsBasedCamera"
    V2X = "V2X"

    InstallPath: Dict[str, str] = {
        FullWaveformLidar: "",
        PBRadar: "",
        PointCloudLidar: "",
        Probabilistic: "",
        PhysicsBasedCamera: "",
        V2X: "",
    }

    SensorGenerator: Dict[str, List[str]] = {
        FullWaveformLidar: [
            "./sensors/Fullwaveformlidar.py",
            "./generators/FullwaveformlidarGenerator.py"],
        PBRadar: [
            "",
            ""],
        PointCloudLidar: [
            "./sensors/Pointcloudlidar.py",
            "./generators/PointcloudlidarGenerator.py"],
        Probabilistic: [
            "",
            ""],
        PhysicsBasedCamera: [
            "./sensors/Physicsbasedcameraunreal.py",
            "./generators/PhysicsbasedcameraunrealGenerator.py"],
        V2X: [
            "",
            ""],
    }


plugins = _Plugins()


def _get_plugins(env: str):
    if platform.system() not in ["Windows", "Linux"]:
        raise Exception(f"Unsupported platform: {platform.system()}")
    spliter = "\\" if platform.system() == "Windows" else "/"
    comment_out = "::" if platform.system() == "Windows" else "#"
    with open(env, "rt") as file:
        while True:
            line = file.readline()
            if line == "":
                break
            else:
                for plugin_guess, _ in plugins.InstallPath.items():
                    if not line.startswith(comment_out) and "Prescan_plugin_path" in line and plugin_guess in line:
                        path = line.split(spliter)
                        for plugin_folder in path:
                            if plugin_guess in plugin_folder:
                                # here is exception for PhysicsBasedCamera, as it's api not in plugin folder
                                if plugin_guess == plugins.PhysicsBasedCamera:
                                    plugin_folder = plugin_guess
                                plugins.InstallPath[plugin_guess] = plugin_folder
        print(plugins.InstallPath)


def _set_python_api():
    with open("./rename_api_namespace/prescan_python_dmapi_template.py", "rt") as file:
        text = file.readlines()
        for import_api in text:
            if import_api.startswith("#"):
                for plugin_guess, folder_name in plugins.InstallPath.items():
                    if folder_name:
                        if f"#{plugin_guess}#" in import_api:
                            import_api_changed = text[text.index(import_api)].replace(f"#{plugin_guess}#", "import")
                            text[text.index(import_api)] = import_api_changed
                            if f"#Folder#" in import_api_changed:
                                import_api_plugin = text[text.index(import_api_changed)].replace("#Folder#",
                                                                                                 folder_name)
                                text[text.index(import_api_changed)] = import_api_plugin
                            continue
        with open("./rename_api_namespace/prescan_python_dmapi.py", "wt") as file:
            file.writelines(text)


def _comment_file(file: str):
    """
    TODO: comment the plugin
    :param file:
    :return:
    """
    with open(file, "rt") as file_sensor:
        text = file_sensor.readlines()
        index = text.index("IS_PLUGIN = True\n")
        if text[index + 1] != comment:
            """avoid duplicated comment"""
            text.insert(index + 1, comment)
            for i in range(index + 2, len(text)):
                text[i] = "#" + text[i]
    with open(file, "wt") as file_sensor:
        file_sensor.writelines(text)


def _uncomment_file(file: str):
    """
    TODO: uncomment the plugin
    :param file: 
    :return: 
    """
    with open(file, "rt") as file_sensor:
        text = file_sensor.readlines()
        index = text.index("IS_PLUGIN = True\n")
        if text[index + 1] == comment:
            text.pop(index + 1)
            for i in range(index + 1, len(text)):
                text[i] = text[i][1:]
    with open(file, "wt") as file_sensor:
        file_sensor.writelines(text)


def _enable_disable_sensor_generator():
    for plugin_guess, folder_name in plugins.InstallPath.items():
        sensor = plugins.SensorGenerator[plugin_guess][0]
        generator = plugins.SensorGenerator[plugin_guess][1]
        if sensor and generator:
            if folder_name:
                _uncomment_file(sensor)
                _uncomment_file(generator)
            else:
                """comment the plugin"""
                _comment_file(sensor)
                _comment_file(generator)


def config_python_api(env: str = "set_env.bat" if platform.system() == "Windows" else "set_env.bash"):
    _get_plugins(env)
    _set_python_api()
    _enable_disable_sensor_generator()


if __name__ == '__main__':
    config_python_api()

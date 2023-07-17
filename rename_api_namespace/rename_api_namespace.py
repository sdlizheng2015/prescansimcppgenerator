#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com


"""This module helps to rename Prescan python api namespace in PyCharm.

Note:
    This module does not create new prescan api but just rename them, to avoid namespace
    conflicts in Pycharm. So that you can vist the api and source code just in a similar way,
    e.g., use prescan_api_experiment.Experiment() instead of prescan.api.experiment.Experiment()

How to use:
    1.  Install PyCharm: PyCharm 2022 and better add "Open Folder as Project" when installing;

    2.  Configure PyCharm: Put this module in you prescan project folder, the same level as .pb
        file. It is recommended to run PyCharm as administrator and then open the project in
        PyCharm menubar: "File-> Open -> you project folder", instead of "Open Folder as Project".

        NOTE: It may cause configure filure if you directly open folder as a project due to the
        authorization of adminstator, even though the system would ask you for admin root.

        Prescan/PyCharm Project Folder:
        │  demo.pb
        │  scripts.py
        │  pycharm_prescan_api.py
        │
        ├─.idea
        │  │  .gitignore
        │  │  encodings.xml
        │  │  misc.xml
        │  │  modules.xml
        │  │  python.iml
        │  │  workspace.xml
        │  │
        │  └─inspectionProfiles
        │          profiles_settings.xml
        │
        └─rename_api_namespace
                rename_api_namespace.py
                __init__.py

    3.  Go to rename_api_namespace.py and follow the user steps 1 and 2;

    4.  After the user configurations, first call config_project_api_path(project_name)
        or config_pycharm_api_path(pycharm_options) in your scripts.py

        We strongly recommend config_pycharm_api_path, this function will permanently add prescan
        api path to your PyCharm options which works in all PyCharm projects.

        However, config_pycharm_api_path can only add prescan api path to your current project,
        namely, in the hidden .idea folder of the current project.

        When you config api path for the first time, there may be an info of "Add Prescan python api
        path to project successfully" or "Add Prescan python api path to Pycharm successfully",
        depending on you called config function, this means the api path may be configured correctly.

        You need to restart PyCharm if you use config_pycharm_api_path and wait for seconds before
        PyCharm is well configured. For config_project_api_path, you don't.

        NOTE: DO NOT USE THEM TOGETHER.

        Example(in scripts.py):
        --------
        >>> import rename_api_namespace

        >>> rename_api_namespace.config_pycharm_api_path(rename_api_namespace.pycharm_options)

        >>> 'Restart PyCharm and wait for seconds, you can check the api paths in PyCharm Interpreter Path'

    5.  Finally, call rename_prescan_python_api_namespace() to generate pycharm_prescan_api module,
        called pycharm_prescan_api.py, located in the same folder as this module. Then, you can
        import pycharm_prescan_api to use prescan python api in PyCharm.

        Example(in scripts.py):
        --------
        >>> import rename_api_namespace

        >>> rename_api_namespace.rename_prescan_python_api_namespace()


    6. Enjoy.
        Example(in scripts.py):
        --------
        >>> from pycharm_prescan_api import *


        >>> experiment: prescan_api_experiment.Experiment = prescan_api_experiment.loadExperimentFromFile("demo.pb")
        >>> car: prescan_api_types.WorldObject = experiment.createObject("Audi_A8_Sedan")
        >>> camera: prescan_api_camera.CameraSensor = prescan_api_camera.createCameraSensor(car)
        >>> camera.pose.position.setXYZ(2, 0, 1.4)
        >>> viewer: prescan_api_viewer.Viewer = prescan_api_viewer.createViewer(experiment)
        >>> viewer.assignCamera(camera)
        >>> >experiment.saveToFile("demo.pb")



"""

import os
import importlib
from typing import List, Tuple
import xml.dom.minidom
from xml.dom.minidom import Document
from xml.dom.minidom import Element
import sys
import ctypes

__all__ = ["rename_prescan_python_api_namespace", "config_project_api_path"]

# ==========================USER NEED DO: step 1========================== #
"""------------------------Configurations by user------------------------"""
prescan_installation_dir = r"D:\Simcenter Prescan\Prescan_2302"
LD_LIB_PATH = r";D:\Simcenter Prescan\Prescan_2302\bin"
python_path = r"D:\Simcenter Prescan\Prescan_2302\python"
module_path = r"D:\Simcenter Prescan\Prescan_2302\modules"
plugin_path = r"D:\Simcenter Prescan\Prescan_2302\Plugins"
pycharm_options = r"C:\Users\yiyan5ez\AppData\Roaming\JetBrains\PyCharmCE2022.2\options"

# ==========================USER NEED DO: step 2========================== #
"""-----------------------------User to check-----------------------------"""
f"""Prescan version 2302, make sure these packages located in {python_path}"""
API_Pkgs = (
    "prescan.api",
    "prescan.api.experiment",
    "prescan.api.types",
    "prescan.api.log",
    "prescan.genericmodel",
    "prescan.internal",
    "prescan.assets",
    "prescan.assets.bufferdata",
)

fr"""make sure 'types' in:{prescan_installation_dir}\modules\GraphBasedRoads\python_interface\prescan\api\roads"""
exceptions = [("GraphBasedRoads.python_interface.prescan.api.roads.types", "prescan_api_roads_types")]

fr"""make sure these separators involved in:
{prescan_installation_dir}\modules\'module_name'\python_interface\prescan\...
{prescan_installation_dir}\Plugins\'plugin_name'\modules\pbradar\python_interface\prescan\...\
{prescan_installation_dir}\python\prescan\...
"""
module_plugin_API_separator = "python_interface"
python_API_separator = "\\python\\"
module_dir_separator = "\\modules\\"
plugin_dir_separator = "\\plugins\\"  # Note: 'p' islower
# ======================================================================== #


# ==============================DO NOT EDIT=============================== #
"""-----------DO NOT EDIT UNLESS YOU KNOW WHAT YOU ARE DOING-------------"""

IS_PYTHON_3 = sys.version_info[0] >= 3
IS_PYTHON_3_8_Later = sys.version_info[0] >= 3 and 8 <= sys.version_info[1]

if not IS_PYTHON_3_8_Later:
    raise Exception("Python version not compatible, as least 3.8 or later")

os.environ["PATH"] += LD_LIB_PATH

Copyright = "# To generate Prescan python import api in PyCharmCE2022.1\n" \
            "# 1. This file is generated automatically;\n" \
            "# 2. All rights reserved by Siemens Prescan;\n" \
            "# 3. Any questions email at yiyyyang@siemens.com.\n" \
            "# 4. Copyright 2023 Siemens\n"


def _right_py_clarification(file):
    file.write("# " + "#" * 70 + "\n")
    file.write(Copyright)
    file.write("# " + "#" * 70 + "\n" * 2)


def _set_environ(file):
    file.write("import os\n")
    file.write("os.environ['PATH'] += r" + "'" + LD_LIB_PATH + "'" + "\n")
    file.write(2 * "\n")


def config_project_api_path(project_name: str = ""):
    """
    TODO: TODO: to add prescan python api path to Pycharm current project options,
          typically: local project ".idea.{project_name}.iml" file
    :param project_name: current project name
    :return:
    """
    import warnings
    warnings.warn('config_project_api_path() is recommended for temporary use, it is '
                  'not safe especially when you delete the pycharm project options.'
                  'For long-term and stable, config_pycharm_api_path() is suggested',
                  DeprecationWarning, stacklevel=2)
    assert project_name, "Must specify the project name"
    write_new_cfg = False
    dom: Document = xml.dom.minidom.parse(f".idea/{project_name}.iml")
    module: Element = dom.documentElement
    component: Element = module.getElementsByTagName("component")[0]
    contents: List[Element] = component.getElementsByTagName("content")
    paths = [dict(content.attributes.items())["url"] for content in contents]
    prescan_api_path = ["file://" + path.replace('\\', '/')
                        for path in (python_path, module_path, plugin_path)]
    for path in prescan_api_path:
        if path not in paths:
            write_new_cfg = True
            api_path: Element = dom.createElement("content")
            api_path.setAttribute("url", path)
            component.appendChild(api_path)
    if write_new_cfg:
        try:
            with open(f".idea/{project_name}.iml", "wt") as file:
                dom.writexml(file)
        except (FileExistsError, FileNotFoundError):
            raise Exception(u"pycharm .idea/Test.iml not found, "
                            u"please create a complete pycharm project")
        print("Add Prescan python api path to project successfully")


def _is_admin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False


def config_pycharm_api_path(options: str = ""):
    """
    TODO: to add prescan python api path to Pycharm options
    :param options:
    :return:
    """
    assert options, "Must specify the pycharm jdk.table.xml"
    write_new_cfg = False
    dom: Document = xml.dom.minidom.parse(f"{options}" + r"\jdk.table.xml")
    application: Element = dom.documentElement
    component: Element = application.getElementsByTagName("component")[0]
    jdk: Element = component.getElementsByTagName("jdk")[0]
    additional: Element = jdk.getElementsByTagName("additional")[0]
    PATHS_BY_USER: List[Element] = additional.getElementsByTagName("PATHS_ADDED_BY_USER_ROOT")
    paths = [dict(PATH_BY_USER.attributes.items())["PATH_ADDED_BY_USER"]
             for PATH_BY_USER in PATHS_BY_USER]
    prescan_api_path = ["file://" + path.replace('\\', '/')
                        for path in (python_path, module_path, plugin_path)]
    for path in prescan_api_path:
        if path not in paths:
            write_new_cfg = True
            api_path: Element = dom.createElement("PATHS_ADDED_BY_USER_ROOT")
            api_path.setAttribute("PATH_ADDED_BY_USER", path)
            additional.appendChild(api_path)
    if write_new_cfg:
        if _is_admin():
            try:
                with open(f"{options}" + r"\jdk.table.xml", "wt") as file:
                    dom.writexml(file)
            except (FileExistsError, FileNotFoundError):
                raise Exception(u"Pycharm jdk.table.xml not found")
            else:
                print("Add Prescan python api path to Pycharm successfully")
        else:
            ctypes.windll.shell32.ShellExecuteW(
                None, "runas", sys.executable, __file__, None, 1)


def _get_import_pkgs(path: str, module_dir: str) -> List[Tuple[str, str]]:
    """
    TODO:
    :param path:
    :param module_dir:
    :return: List[Tuple[import_name, as_name]]
    """
    import_path = path.split(module_dir)[-1].replace("\\", ".") + "."
    module_pkgs = [package for package in os.listdir(path) if os.path.isdir(path + "\\" + package)]
    module_api = [(import_path + module_pkg, "_".join((import_path + module_pkg).split(".")[-3:]))
                  for module_pkg in module_pkgs]
    return module_api


def rename_prescan_python_api_namespace():
    """
    TODO:
    :return:
    """
    module_api: List[Tuple[str, str]] = []  # API for plugin python
    plugin_api: List[Tuple[str, str]] = []  # API for module python
    python_api: List[Tuple[str, str]] = []  # API for default python
    with open("./prescan_python_dmapi.py", "wt") as file:
        for pkg in API_Pkgs:
            modular_paths = importlib.import_module(pkg, package=python_path).__path__
            for path in modular_paths:  # type: str
                path = path.replace("/", "\\")
                if python_API_separator in path and module_plugin_API_separator not in path:
                    import_path = path.split(python_API_separator)[-1].replace("\\", ".")
                    python_api.append((import_path, import_path.replace(".", "_")))
                elif module_plugin_API_separator in path:
                    if module_dir_separator in path and plugin_dir_separator not in path:
                        module_api.extend(_get_import_pkgs(path, module_dir_separator))
                    elif plugin_dir_separator in path:
                        plugin_api.extend(_get_import_pkgs(path, plugin_dir_separator))
                    else:
                        raise Exception("unresolved package path for package")
                else:
                    pass
        _right_py_clarification(file)
        _set_environ(file)
        for api in python_api + module_api + plugin_api + exceptions:
            file.write("import " + api[1].replace("_", ".") + "\n")
        for api in python_api + module_api + plugin_api + exceptions:
            file.write("import " + api[0] + " as " + api[1] + "\n")

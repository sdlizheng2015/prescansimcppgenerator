#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com
import os
import sys

from utils.config_python_api import config_python_api
import argparse
from logger.UserLog import uniLog

_argparse = argparse.ArgumentParser()
_argparse.add_argument("-pb_dir", type=str, action="store", default=os.environ['experiment_pb'],
                       help=r'''Path to pb file, default: ${experiment_pb}, 
                       usage: -pb_dir "C:\Users\yiyan5ez\Desktop\SimcppGenerator\SimcppGenerator.pb"''')

_argparse.add_argument("-out_dir", type=str, action="store", default=os.environ['simcpp_dir'],
                       help=r'''folder to save generated simcpp project, default: ${simcpp_dir}, 
                       usage: -out_dir "C:\Users\yiyan5ez\Desktop\SimcppGenerator\simcpp"''')

_argparse.add_argument("-all_ports", type=int, action="store", default=1,
                       help="Enable all sensors output ports, default: 1(enabled), to disabled: -all_ports 0")

_argparse.add_argument("-show_time", type=int, action="store", default=1,
                       help="print simulation time and cyclic time, default: 1(enabled), to disabled: -show_time 0")

_argparse.add_argument("-options", type=str, action="store", default="",
                       help="set simulation customOptions, default: '', "
                            "valid values: 'federate_record=1', 'profiler=file'"
                            ", e.g., -options profiler=file")

_argparse.add_argument("-conf_api", type=int, action="store", default=1,
                       help="based on set_env file, automatically, config Prescan_python_dmapi.py and "
                            "modify python files in sensors and generators folders. You must make sure "
                            "all configs in set_env files are correct, default: 1(enabled), to disabled: -conf_api 0")

_argparse.add_argument("-vis", type=int, action="store", default=0,
                       help="to visualize the image or lidar point, you need opencv installed on your machine first, "
                            "default: 0(disabled), to enable: -vis 1")

_argparse.add_argument("-driver", type=str, action="store", default='',
                       help="to add path follower for pre-configured Amesim dynamics with active trajectory, "
                            "default: '', usage: -driver VehicleName1,VehicleName2,...")

_argparse.add_argument("-bridge", type=int, action="store", default=0,
                       help="enable simcpp bridge(not used now), default: 0, e.g., -bridge 1")

_argparse.add_argument("-load_yaml", type=int, action="store", default=0,
                       help="Load pb yaml configs(not used now)")

_argparse.add_argument("-yaml", type=str, action="store", default="",
                       help="yaml path (not used now)")

if __name__ == '__main__':
    args = _argparse.parse_args()
    pb_dir = args.pb_dir
    dst = args.out_dir
    all_ports = bool(args.all_ports)
    show_time = bool(args.show_time)
    bridge = bool(args.bridge)
    options = args.options
    vis = bool(args.vis)
    driver: str = args.driver
    followers = [name for name in driver.split(",") if name]

    """
    The following is to prepare and normally run the generator
    config_python_api must be done before construct SimcppGenerator
    """
    if bool(args.conf_api):
        try:
            config_python_api()
        except Exception as ee:
            print(f"auto config api failed: {ee}")
            uniLog.logger.error(f"auto config api failed: {ee}")
            sys.exit()

    from SimcppGenerator import SimcppGenerator

    generator = SimcppGenerator(
        pb=fr"{pb_dir}",
        pb_yaml="",
        load_yaml=False,
        enable_all_port=all_ports,
        enable_sim_time=show_time,
        enable_bridge=bridge,
        options=options,
        vis=vis,
        followers=followers,
        dst=dst
    )

    generator.generate()

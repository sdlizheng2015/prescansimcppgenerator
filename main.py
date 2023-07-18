#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from SimcppGenerator import SimcppGenerator
import argparse

_argparse = argparse.ArgumentParser()
_argparse.add_argument("pb_dir", type=str,
                       help=r'''Path pb file, e.g., "C:\Users\yiyan5ez\Desktop\SimcppGenerator\SimcppGenerator.pb"''')
_argparse.add_argument("ps_dir", type=str,
                       help=r'''Prescan_DIR path, e.g., "D:\Simcenter Prescan\Prescan_2302\lib\cmake"''')
_argparse.add_argument("-out_dir", type=str, action="store", default="./projects",
                       help="folder to save generated simcpp project, default: ./projects, e.g.,: -out_dir ./projects")
_argparse.add_argument("-all_ports", type=int, action="store", default=0,
                       help="Enable all sensors output ports, default: 0, e.g., -all_ports 1")
_argparse.add_argument("-load_yaml", type=int, action="store", default=0,
                       help="Load pb yaml configs(not used now)")
_argparse.add_argument("-yaml", type=str, action="store", default="",
                       help="yaml path (not used now)")

if __name__ == '__main__':
    args = _argparse.parse_args()
    pb_dir = args.pb_dir
    ps_dir: str = args.ps_dir
    ps_dir = ps_dir.replace("\\", "\\\\")
    dst = args.out_dir
    all_ports = bool(args.all_ports)

    generator = SimcppGenerator(
        pb=fr"{pb_dir}",
        pb_yaml="",
        ps_dir=fr"{ps_dir}",
        load_yaml=False,
        enable_all_port= all_ports
    )

    generator.copy_to_project(fr"{dst}")
    generator.generate_codes()
    generator.write_to_project()

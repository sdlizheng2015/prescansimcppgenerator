#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

import os
import re
import time
import datetime
import logging
import sys
import yaml

__all__ = ["uniLog", "Log"]


class _AutoDeleteLog:
    def __init__(self, path: str, re_name: str, days: int, num: int, cur_log: logging.Logger):
        """
        TODO:
        :param path: log文件的目录
        :param re_name: log文件命名的关键字
        :param days: 删除多少天内的log
        """
        self.file_dir = path
        self.file_re = re_name
        self.expire_time = days
        self._num = num
        self._log = cur_log
        if self._find_file():
            self._delete_logs()

    def _find_file(self):
        """
        TODO:
        :return:
        """
        try:
            all_file = os.listdir(self.file_dir)
        except Exception as ee:
            self._log.warning(f"can not find logs to be deleted: {ee}")
            return False
        else:
            # 匹配正则的文件
            self.reg_file_list = []
            reg_str = self.file_re
            for reg_file in all_file:
                if re.match(reg_str, reg_file):
                    self.reg_file_list.append(reg_file)
            return True if len(self.reg_file_list) >= self._num else False

    def _delete_logs(self):
        """
        TODO:
        :return:
        """
        today = datetime.datetime.now()
        # n天
        n_days = datetime.timedelta(days=int(self.expire_time))
        # n天前日期
        n_days_agos = today - n_days
        # n天前时间戳
        n_days_agos_timestamps = time.mktime(n_days_agos.timetuple())

        for date_file in self.reg_file_list:
            abs_file = os.path.join(self.file_dir, date_file)
            file_timestamp = os.path.getmtime(abs_file)

            if float(file_timestamp) <= float(n_days_agos_timestamps):
                if os.path.isfile(abs_file):
                    os.remove(abs_file)


class Log:
    def __init__(self, log_path: str, log_name: str, config_file: str):
        _date = time.strftime('%Y_%m_%d', time.localtime())
        self._logger: logging.Logger = logging.getLogger(__name__)
        _process_logHandle = logging.FileHandler(filename=f"{log_path}{log_name}_{_date}.log")
        _format = logging.Formatter(
            fmt='%(asctime)s %(message)s',
            datefmt='%m/%d/%Y %I:%M:%S %p')
        _process_logHandle.setFormatter(_format)
        self._logger.addHandler(_process_logHandle)
        self._logger.setLevel(logging.INFO)

        try:
            with open(f"{config_file}", "r") as file:
                config = yaml.load(file, Loader=yaml.FullLoader)
                _debug = config["debug"]
                _autoDelLog, days, _num = \
                    config["log"]["autodel"], \
                    config["log"]["keepdays"], \
                    config["log"]["num"]
        except Exception as ee:
            self._logger.warning(f"public yaml config file open failed: {ee}")
            sys.exit(1)
        else:
            if _autoDelLog:
                _AutoDeleteLog(f"{log_path}", f"{log_name}", days, _num, self._logger)

    @property
    def logger(self):
        return self._logger


uniLog = Log(log_path="./log/", log_name="unilog",
             config_file="./logger/config.yaml")

# ######################################################################
# To convert CPP data define header to CPython type
# 1. This file is generated automatically;
# 2. All rights reserved by Yang;
# 3. Now only Struct and #define constant are supported;
# 4. Any questions email at yyangjlu@163.com.
# ######################################################################
import copy
import sys
import time
from typing import List
from datatype.base import PackedStructure
from ctypes import *
import mmap
import os
from google.protobuf.message import Message
import platform

class ShmHeader(PackedStructure):
    _fields_ = [
        ("shm_writable", c_uint8),  # //
        ("version", 20 * c_char),  # //
        ("msg_name", 20 * c_char),  # //
        ("msg_length", c_uint32),  # //
    ]


shmHeader = ShmHeader()


class ShmHandler:
    if platform.system() == "Windows":
        shm_path = "C:\\ProgramData\\boost_interprocess\\44000000\\"
    elif platform.system() == "Linux":
        shm_path = "/dev/shm/"
    else:
        raise Exception("Platform not supported")

    def __init__(self, shm_name: str, buffer_size: int):
        self.shm_name = shm_name
        self._buffer_size = buffer_size
        self._shmHeader = ShmHeader()
        self._h_len = len(self._shmHeader)
        self._ptext = 0  # pointer to now write/read position in shm
        self._shm_file_path_name = ShmHandler.shm_path + self.shm_name
        self._shm_file = None
        self._shm_addr = None

    def init_shm(self, shm_writable: bool) -> bool:
        try:
            if not os.path.exists(self._shm_file_path_name):
                with open(self._shm_file_path_name, "w+b") as _shm_file:
                    _shm_file.write(b"\x00" * self._buffer_size)
                    print(f"{self.shm_name} shared memory created")
            self._shm_file = open(self._shm_file_path_name, "r+b")
            print(f"{self.shm_name} shared memory opened")
            self._shm_addr = mmap.mmap(self._shm_file.fileno(), 0)
            self._shm_addr.seek(0)
            if shm_writable:
                self._set_shm_writable()
        except Exception as ee:
            print(ee)
            return False
        else:
            return True

    def write_shm(self, shm_header: ShmHeader, message: Message,
                  timeout: float = 0.1, blocking: bool = False) -> bool:
        start_time = time.time()
        while True:
            if self._get_shm_writable():
                self._set_shm_writable()
                break
            else:
                time.sleep(10 ** -6)
                if time.time() - start_time > timeout:
                    if blocking:
                        pass
                    else:
                        break
        if not self._get_shm_writable():
            print(f"{self.shm_name} write shm failed")
            return False
        else:
            self._shmHeader.msg_name = shm_header.msg_name
            self._shmHeader.version = shm_header.version
            self._shmHeader.msg_length = message.ByteSize()
            self._shm_addr.seek(0)
            """
            Here must run the simulation first to determine the buffer size
            for each shared memory. If the actual data size exceeds the buffer, 
            the shm read and protobuf parse will have error.
            """
            if self._shmHeader.msg_length + self._h_len > self._buffer_size:
                self.free_shm()
                raise Exception(f"{self.shm_name}: write protobuf message length "
                                f"over buffer size, can't serialize")
            self._shm_addr.write(self._shmHeader.encode() + message.SerializeToString())
            self._set_shm_readable()
            return True

    def read_shm(self, shm_header: ShmHeader, message: Message,
                 timeout: float = 0.1, blocking: bool = False) -> bool:
        start_time = time.time()
        # print(f"{self.shm_name}: ", self._get_shm_writable())
        while True:
            if not self._get_shm_writable():
                self._set_shm_readable()
                break
            else:
                time.sleep(10 ** -6)
                if time.time() - start_time > timeout:
                    if blocking:
                        pass
                    else:
                        break
        if self._get_shm_writable():
            print(f"{self.shm_name} read shm failed")
            return False
        else:
            self._shm_addr.seek(0)
            data = self._shm_addr.read()
            self._shmHeader.decode(data[0: self._h_len])
            shm_header.msg_name = self._shmHeader.msg_name
            shm_header.version = self._shmHeader.version
            shm_header.shm_writable = self._shmHeader.shm_writable
            shm_header.msg_length = self._shmHeader.msg_length
            # try:
            """
            Here must run the simulation first to determine the buffer size
            for each shared memory. If the actual data size exceeds the buffer, 
            the shm read and protobuf ParseFromString will have error.
            """
            if self._shmHeader.msg_length + self._h_len > self._buffer_size:
                self.free_shm()
                raise Exception(f"{self.shm_name}: read protobuf message length "
                                f"over buffer size, can't parse")
            message.ParseFromString(data[self._h_len:self._h_len + self._shmHeader.msg_length])
            self._set_shm_writable()
            return True

    def free_shm(self):
        try:
            self._shm_addr.close()
            self._shm_file.close()
        except Exception as ee:
            print(ee)
            print(f"free shm {self.shm_name} failed")
        else:
            print(f"free shm {self.shm_name} succeed")

    def _get_shm_writable(self) -> bool:
        self._shm_addr.seek(0)
        data = self._shm_addr.read()
        self._shmHeader.decode(data[0: self._h_len])
        return bool(self._shmHeader.shm_writable)

    def _set_shm_writable(self):
        self._shmHeader.shm_writable = 1
        self._shm_addr.seek(0)
        self._shm_addr.write(self._shmHeader.encode())

    def _set_shm_readable(self):
        self._shmHeader.shm_writable = 0
        self._shm_addr.seek(0)
        self._shm_addr.write(self._shmHeader.encode())


class HandlerManager:
    def __init__(self):
        self._shm_handlers: List[ShmHandler] = []

    def initial_shmhandlers(self, shm_writable: bool):
        initialized = True
        for handler in self._shm_handlers:
            if not handler.init_shm(shm_writable):
                initialized = False
                print(f"{handler.shm_name} initial failed")
                handler.free_shm()
        if not initialized:
            self.free_shmhandlers()
            raise Exception("handler manager initial failed")

    def free_shmhandlers(self):
        for handler in self._shm_handlers:
            handler.free_shm()

    def add_handler(self, shm_handler: ShmHandler):
        self._shm_handlers.append(shm_handler)

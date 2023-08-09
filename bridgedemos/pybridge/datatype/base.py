# !/usr/bin/env python
# -*- coding:utf-8 -*-
# project:    bridge
# author:     OldYoung
# datetime:  2023/6/6 18:41


from ctypes import Structure, string_at, memmove, addressof, sizeof, Array


class PackedStructure(Structure):
    _fields_ = []
    _pack_ = 1

    def __len__(self):
        return len(self.encode())

    def encode(self):
        """
        TODO: pack to bytes stream
        """
        return string_at(addressof(self), sizeof(self))

    def decode(self, data: bytes):
        """
        TODO: to set variables according to bytes stream
        """
        memmove(addressof(self), data, sizeof(self))

    def get_values(self):
        """
        TODO: to get all variable values in structure
        """
        values = {}
        for value in self._fields_:
            attr = self.__getattribute__(value[0])
            if isinstance(attr, Array):
                for i in range(len(attr)):
                    if isinstance(attr[i], PackedStructure):
                        values.setdefault(f"{value[0]}{[i]}", attr[i].get_values())
                    else:
                        values.setdefault(f"{value[0]}{[i]}", self.__getattribute__(f"{value[0]}")[i])
            else:
                if isinstance(attr, PackedStructure):
                    values.setdefault(value[0], attr.get_values())
                else:
                    values.setdefault(value[0], self.__getattribute__(value[0]))
        return values

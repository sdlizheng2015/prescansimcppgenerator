# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: bridge_msgs/localiztion_msgs/localization.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from bridge_msgs.basic_msgs import header_pb2 as bridge__msgs_dot_basic__msgs_dot_header__pb2
from bridge_msgs.basic_msgs import geometry_pb2 as bridge__msgs_dot_basic__msgs_dot_geometry__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n/bridge_msgs/localiztion_msgs/localization.proto\x12\x14prescan.localization\x1a#bridge_msgs/basic_msgs/header.proto\x1a%bridge_msgs/basic_msgs/geometry.proto\"e\n\x03Gps\x12&\n\x06header\x18\x01 \x01(\x0b\x32\x16.prescan.common.Header\x12%\n\x03llh\x18\x02 \x01(\x0b\x32\x18.prescan.common.PointLLH\x12\x0f\n\x07heading\x18\x05 \x01(\x01\"T\n\x04Pose\x12&\n\x06header\x18\x01 \x01(\x0b\x32\x16.prescan.common.Header\x12$\n\x03xyz\x18\x02 \x01(\x0b\x32\x17.prescan.common.Point3D\"\xc5\x01\n\x03Imu\x12&\n\x06header\x18\x01 \x01(\x0b\x32\x16.prescan.common.Header\x12\x30\n\x0borientation\x18\x02 \x01(\x0b\x32\x1b.prescan.common.Orientation\x12\x35\n\x10orientation_rate\x18\x07 \x01(\x0b\x32\x1b.prescan.common.Orientation\x12-\n\x0c\x61\x63\x63\x65leration\x18\x0c \x01(\x0b\x32\x17.prescan.common.Point3D\"\xb6\x01\n\x12LocalizationStatus\x12&\n\x06header\x18\x01 \x01(\x0b\x32\x16.prescan.common.Header\x12&\n\x03gps\x18\x02 \x01(\x0b\x32\x19.prescan.localization.Gps\x12&\n\x03imu\x18\x03 \x01(\x0b\x32\x19.prescan.localization.Imu\x12(\n\x04pose\x18\x04 \x01(\x0b\x32\x1a.prescan.localization.Pose')



_GPS = DESCRIPTOR.message_types_by_name['Gps']
_POSE = DESCRIPTOR.message_types_by_name['Pose']
_IMU = DESCRIPTOR.message_types_by_name['Imu']
_LOCALIZATIONSTATUS = DESCRIPTOR.message_types_by_name['LocalizationStatus']
Gps = _reflection.GeneratedProtocolMessageType('Gps', (_message.Message,), {
  'DESCRIPTOR' : _GPS,
  '__module__' : 'bridge_msgs.localiztion_msgs.localization_pb2'
  # @@protoc_insertion_point(class_scope:prescan.localization.Gps)
  })
_sym_db.RegisterMessage(Gps)

Pose = _reflection.GeneratedProtocolMessageType('Pose', (_message.Message,), {
  'DESCRIPTOR' : _POSE,
  '__module__' : 'bridge_msgs.localiztion_msgs.localization_pb2'
  # @@protoc_insertion_point(class_scope:prescan.localization.Pose)
  })
_sym_db.RegisterMessage(Pose)

Imu = _reflection.GeneratedProtocolMessageType('Imu', (_message.Message,), {
  'DESCRIPTOR' : _IMU,
  '__module__' : 'bridge_msgs.localiztion_msgs.localization_pb2'
  # @@protoc_insertion_point(class_scope:prescan.localization.Imu)
  })
_sym_db.RegisterMessage(Imu)

LocalizationStatus = _reflection.GeneratedProtocolMessageType('LocalizationStatus', (_message.Message,), {
  'DESCRIPTOR' : _LOCALIZATIONSTATUS,
  '__module__' : 'bridge_msgs.localiztion_msgs.localization_pb2'
  # @@protoc_insertion_point(class_scope:prescan.localization.LocalizationStatus)
  })
_sym_db.RegisterMessage(LocalizationStatus)

if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _GPS._serialized_start=149
  _GPS._serialized_end=250
  _POSE._serialized_start=252
  _POSE._serialized_end=336
  _IMU._serialized_start=339
  _IMU._serialized_end=536
  _LOCALIZATIONSTATUS._serialized_start=539
  _LOCALIZATIONSTATUS._serialized_end=721
# @@protoc_insertion_point(module_scope)

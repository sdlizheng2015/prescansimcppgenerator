# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: bridge_msgs/chassis_msgs/chassis.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from bridge_msgs.basic_msgs import header_pb2 as bridge__msgs_dot_basic__msgs_dot_header__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n&bridge_msgs/chassis_msgs/chassis.proto\x12\x0fprescan.chassis\x1a#bridge_msgs/basic_msgs/header.proto\"\xdd\x02\n\x07\x43hassis\x12\x14\n\tspeed_mps\x18\x05 \x01(\x02:\x01\x30\x12\x13\n\x08throttle\x18\x08 \x01(\x02:\x01\x30\x12\x10\n\x05\x62rake\x18\t \x01(\x02:\x01\x30\x12\x13\n\x08steering\x18\x0b \x01(\x02:\x01\x30\x12<\n\rgear_location\x18\x17 \x01(\x0e\x32%.prescan.chassis.Chassis.GearPosition\x12&\n\x06header\x18\x19 \x01(\x0b\x32\x16.prescan.common.Header\x12\x14\n\x0cvehicle_name\x18! \x01(\t\"\x83\x01\n\x0cGearPosition\x12\x10\n\x0cGEAR_NEUTRAL\x10\x00\x12\x0e\n\nGEAR_DRIVE\x10\x01\x12\x10\n\x0cGEAR_REVERSE\x10\x02\x12\x10\n\x0cGEAR_PARKING\x10\x03\x12\x0c\n\x08GEAR_LOW\x10\x04\x12\x10\n\x0cGEAR_INVALID\x10\x05\x12\r\n\tGEAR_NONE\x10\x06')



_CHASSIS = DESCRIPTOR.message_types_by_name['Chassis']
_CHASSIS_GEARPOSITION = _CHASSIS.enum_types_by_name['GearPosition']
Chassis = _reflection.GeneratedProtocolMessageType('Chassis', (_message.Message,), {
  'DESCRIPTOR' : _CHASSIS,
  '__module__' : 'bridge_msgs.chassis_msgs.chassis_pb2'
  # @@protoc_insertion_point(class_scope:prescan.chassis.Chassis)
  })
_sym_db.RegisterMessage(Chassis)

if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _CHASSIS._serialized_start=97
  _CHASSIS._serialized_end=446
  _CHASSIS_GEARPOSITION._serialized_start=315
  _CHASSIS_GEARPOSITION._serialized_end=446
# @@protoc_insertion_point(module_scope)

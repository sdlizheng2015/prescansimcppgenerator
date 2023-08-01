# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: bridge_msgs/perception_msgs/obstacle.proto
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


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n*bridge_msgs/perception_msgs/obstacle.proto\x12\x12prescan.perception\x1a#bridge_msgs/basic_msgs/header.proto\x1a%bridge_msgs/basic_msgs/geometry.proto\"\x9c\x05\n\x08Obstacle\x12\n\n\x02id\x18\x01 \x01(\x05\x12)\n\x08position\x18\x02 \x01(\x0b\x32\x17.prescan.common.Point3D\x12\x0f\n\x07heading\x18\x03 \x01(\x01\x12)\n\x08velocity\x18\x04 \x01(\x0b\x32\x17.prescan.common.Point3D\x12*\n\x03\x62ox\x18\x05 \x01(\x0b\x32\x1d.prescan.common.BoundingBox3D\x12/\n\x04type\x18\n \x01(\x0e\x32!.prescan.perception.Obstacle.Type\x12\x16\n\nconfidence\x18\r \x01(\x01\x42\x02\x18\x01\x12\x36\n\x08sub_type\x18\x13 \x01(\x0e\x32$.prescan.perception.Obstacle.SubType\x12&\n\x06header\x18\x14 \x01(\x0b\x32\x16.prescan.common.Header\"i\n\x04Type\x12\x0b\n\x07UNKNOWN\x10\x00\x12\x13\n\x0fUNKNOWN_MOVABLE\x10\x01\x12\x15\n\x11UNKNOWN_UNMOVABLE\x10\x02\x12\x0e\n\nPEDESTRIAN\x10\x03\x12\x0b\n\x07\x42ICYCLE\x10\x04\x12\x0b\n\x07VEHICLE\x10\x05\"\xdc\x01\n\x07SubType\x12\x0e\n\nST_UNKNOWN\x10\x00\x12\x16\n\x12ST_UNKNOWN_MOVABLE\x10\x01\x12\x18\n\x14ST_UNKNOWN_UNMOVABLE\x10\x02\x12\n\n\x06ST_CAR\x10\x03\x12\n\n\x06ST_VAN\x10\x04\x12\x0c\n\x08ST_TRUCK\x10\x05\x12\n\n\x06ST_BUS\x10\x06\x12\x0e\n\nST_CYCLIST\x10\x07\x12\x13\n\x0fST_MOTORCYCLIST\x10\x08\x12\x11\n\rST_TRICYCLIST\x10\t\x12\x11\n\rST_PEDESTRIAN\x10\n\x12\x12\n\x0eST_TRAFFICCONE\x10\x0b\"n\n\tPointLane\x12&\n\x05point\x18\x01 \x03(\x0b\x32\x17.prescan.common.Point3D\x12\x11\n\tnumericID\x18\x02 \x01(\r\x12&\n\x06header\x18\x03 \x01(\x0b\x32\x16.prescan.common.Header\"a\n\nPointLanes\x12+\n\x04lane\x18\x01 \x03(\x0b\x32\x1d.prescan.perception.PointLane\x12&\n\x06header\x18\x02 \x01(\x0b\x32\x16.prescan.common.Header\"<\n\nPolyNomial\x12\n\n\x02N0\x18\x01 \x01(\x01\x12\n\n\x02N1\x18\x02 \x01(\x01\x12\n\n\x02N2\x18\x03 \x01(\x01\x12\n\n\x02N3\x18\x04 \x01(\x01\"\xac\x03\n\x08PolyLane\x12/\n\x04type\x18\x01 \x01(\x0e\x32!.prescan.perception.PolyLane.Type\x12\x0f\n\x07quality\x18\x02 \x01(\x01\x12\x14\n\x0cmodel_degree\x18\x03 \x01(\x05\x12)\n\x01X\x18\x04 \x01(\x0b\x32\x1e.prescan.perception.PolyNomial\x12)\n\x01Y\x18\x05 \x01(\x0b\x32\x1e.prescan.perception.PolyNomial\x12)\n\x01Z\x18\x06 \x01(\x0b\x32\x1e.prescan.perception.PolyNomial\x12\x0e\n\x06length\x18\x08 \x01(\x01\x12\x0f\n\x07groupID\x18\t \x01(\r\x12\x12\n\nlane_label\x18\n \x01(\r\x12\x11\n\tnumericID\x18\x0b \x01(\r\x12&\n\x06header\x18\x0c \x01(\x0b\x32\x16.prescan.common.Header\"W\n\x04Type\x12\t\n\x05Solid\x10\x00\x12\x08\n\x04\x44\x61sh\x10\x01\x12\x0e\n\nSolidSolid\x10\x02\x12\r\n\tSolidDash\x10\x03\x12\x0c\n\x08\x44\x61shDash\x10\x04\x12\r\n\tDashSolid\x10\x05\"_\n\tPolyLanes\x12*\n\x04lane\x18\x01 \x03(\x0b\x32\x1c.prescan.perception.PolyLane\x12&\n\x06header\x18\x02 \x01(\x0b\x32\x16.prescan.common.Header\"n\n\tObstacles\x12\x39\n\x13perception_obstacle\x18\x01 \x03(\x0b\x32\x1c.prescan.perception.Obstacle\x12&\n\x06header\x18\x02 \x01(\x0b\x32\x16.prescan.common.Header')



_OBSTACLE = DESCRIPTOR.message_types_by_name['Obstacle']
_POINTLANE = DESCRIPTOR.message_types_by_name['PointLane']
_POINTLANES = DESCRIPTOR.message_types_by_name['PointLanes']
_POLYNOMIAL = DESCRIPTOR.message_types_by_name['PolyNomial']
_POLYLANE = DESCRIPTOR.message_types_by_name['PolyLane']
_POLYLANES = DESCRIPTOR.message_types_by_name['PolyLanes']
_OBSTACLES = DESCRIPTOR.message_types_by_name['Obstacles']
_OBSTACLE_TYPE = _OBSTACLE.enum_types_by_name['Type']
_OBSTACLE_SUBTYPE = _OBSTACLE.enum_types_by_name['SubType']
_POLYLANE_TYPE = _POLYLANE.enum_types_by_name['Type']
Obstacle = _reflection.GeneratedProtocolMessageType('Obstacle', (_message.Message,), {
  'DESCRIPTOR' : _OBSTACLE,
  '__module__' : 'bridge_msgs.perception_msgs.obstacle_pb2'
  # @@protoc_insertion_point(class_scope:prescan.perception.Obstacle)
  })
_sym_db.RegisterMessage(Obstacle)

PointLane = _reflection.GeneratedProtocolMessageType('PointLane', (_message.Message,), {
  'DESCRIPTOR' : _POINTLANE,
  '__module__' : 'bridge_msgs.perception_msgs.obstacle_pb2'
  # @@protoc_insertion_point(class_scope:prescan.perception.PointLane)
  })
_sym_db.RegisterMessage(PointLane)

PointLanes = _reflection.GeneratedProtocolMessageType('PointLanes', (_message.Message,), {
  'DESCRIPTOR' : _POINTLANES,
  '__module__' : 'bridge_msgs.perception_msgs.obstacle_pb2'
  # @@protoc_insertion_point(class_scope:prescan.perception.PointLanes)
  })
_sym_db.RegisterMessage(PointLanes)

PolyNomial = _reflection.GeneratedProtocolMessageType('PolyNomial', (_message.Message,), {
  'DESCRIPTOR' : _POLYNOMIAL,
  '__module__' : 'bridge_msgs.perception_msgs.obstacle_pb2'
  # @@protoc_insertion_point(class_scope:prescan.perception.PolyNomial)
  })
_sym_db.RegisterMessage(PolyNomial)

PolyLane = _reflection.GeneratedProtocolMessageType('PolyLane', (_message.Message,), {
  'DESCRIPTOR' : _POLYLANE,
  '__module__' : 'bridge_msgs.perception_msgs.obstacle_pb2'
  # @@protoc_insertion_point(class_scope:prescan.perception.PolyLane)
  })
_sym_db.RegisterMessage(PolyLane)

PolyLanes = _reflection.GeneratedProtocolMessageType('PolyLanes', (_message.Message,), {
  'DESCRIPTOR' : _POLYLANES,
  '__module__' : 'bridge_msgs.perception_msgs.obstacle_pb2'
  # @@protoc_insertion_point(class_scope:prescan.perception.PolyLanes)
  })
_sym_db.RegisterMessage(PolyLanes)

Obstacles = _reflection.GeneratedProtocolMessageType('Obstacles', (_message.Message,), {
  'DESCRIPTOR' : _OBSTACLES,
  '__module__' : 'bridge_msgs.perception_msgs.obstacle_pb2'
  # @@protoc_insertion_point(class_scope:prescan.perception.Obstacles)
  })
_sym_db.RegisterMessage(Obstacles)

if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _OBSTACLE.fields_by_name['confidence']._options = None
  _OBSTACLE.fields_by_name['confidence']._serialized_options = b'\030\001'
  _OBSTACLE._serialized_start=143
  _OBSTACLE._serialized_end=811
  _OBSTACLE_TYPE._serialized_start=483
  _OBSTACLE_TYPE._serialized_end=588
  _OBSTACLE_SUBTYPE._serialized_start=591
  _OBSTACLE_SUBTYPE._serialized_end=811
  _POINTLANE._serialized_start=813
  _POINTLANE._serialized_end=923
  _POINTLANES._serialized_start=925
  _POINTLANES._serialized_end=1022
  _POLYNOMIAL._serialized_start=1024
  _POLYNOMIAL._serialized_end=1084
  _POLYLANE._serialized_start=1087
  _POLYLANE._serialized_end=1515
  _POLYLANE_TYPE._serialized_start=1428
  _POLYLANE_TYPE._serialized_end=1515
  _POLYLANES._serialized_start=1517
  _POLYLANES._serialized_end=1612
  _OBSTACLES._serialized_start=1614
  _OBSTACLES._serialized_end=1724
# @@protoc_insertion_point(module_scope)

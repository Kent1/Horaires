#!/usr/bin/python
# encoding: utf-8

from ctypes import *


class Room(Structure):
    _fields_ = [
        ("room_id",     c_uint16),
        ("room_type",   c_uint8),
        ("faculty",     c_uint8),
        ("capacity",    c_uint16),
        ("assignation", POINTER(c_uint16)),
    ]


class ArrayRooms(Structure):
    _fields_ = [
        ("data", POINTER(POINTER(Room))),
        ("size", c_size_t),
    ]


class MatrixRooms(Structure):
    _fields_ = [
        ("data", POINTER(POINTER(POINTER(POINTER(Room))))),
        ("size", POINTER(POINTER(c_size_t))),
    ]


class Exam(Structure):
    _fields_ = [
        ("exam_id",        c_uint16),
        ("faculty",        c_uint8),
        ("teacher_id",     c_uint32),
        ("students",       POINTER(c_uint32)),
        ("enrollment",     c_uint16),
        ("room_type",      c_uint8),
        ("room_id",        c_uint16),
        ("timeslot",       c_uint8),
        ("availabilities", POINTER(c_bool)),
        ("conflicts",      POINTER(c_bool)),
        ("deps",           POINTER(c_uint16)),
    ]


class ArrayExams(Structure):
    _fields_ = [
        ("data", POINTER(POINTER(Exam))),
        ("size", c_size_t),
    ]

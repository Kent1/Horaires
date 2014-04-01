import ctypes
p = ctypes.POINTER


class Room(ctypes.Structure):
    NOT_ASSIGNED = 65535

    _fields_ = [
        ("room_id",     ctypes.c_uint16),
        ("room_type",   ctypes.c_uint8),
        ("faculty",     ctypes.c_uint8),
        ("capacity",    ctypes.c_uint16),
        ("assignation", p(ctypes.c_uint16)),
    ]


class ArrayRooms(ctypes.Structure):
    _fields_ = [
        ("data", p(p(Room))),
        ("size", ctypes.c_size_t),
    ]


class MatrixRooms(ctypes.Structure):
    _fields_ = [
        ("data", p(p(p(p(Room))))),
        ("size", p(p(ctypes.c_size_t))),
    ]


class Exam(ctypes.Structure):
    _fields_ = [
        ("exam_id",        ctypes.c_uint16),
        ("faculty",        ctypes.c_uint8),
        ("teacher_id",     ctypes.c_uint32),
        ("students",       p(ctypes.c_uint32)),
        ("enrollment",     ctypes.c_uint16),
        ("room_type",      ctypes.c_uint8),
        ("room_id",        ctypes.c_uint16),
        ("timeslot",       ctypes.c_uint8),
        ("availabilities", p(ctypes.c_bool)),
        ("conflicts",      p(ctypes.c_bool)),
        ("deps",           p(ctypes.c_uint16)),
        ("deps_size",      ctypes.c_uint8),
    ]


class ArrayExams(ctypes.Structure):
    _fields_ = [
        ("data", p(p(Exam))),
        ("size", ctypes.c_size_t),
    ]

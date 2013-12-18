#!/usr/bin/python
# encoding: utf-8

# Python imports
import os
from ctypes import *

# ExamTimetable imports
from examtimetable import c_structs

# Load libtimetable librairy
MAIN_DIR_PATH = 'lib/libtimetable'
if os.uname()[0] == 'Darwin':
    lib = cdll.LoadLibrary(MAIN_DIR_PATH + '.dylib')
elif os.uname()[0] == 'Linux':
    lib = cdll.LoadLibrary(MAIN_DIR_PATH + '.so')
else:
    raise Exception('OS not supported')

# exam *init_exam(uint16_t exam_id, uint8_t faculty, uint32_t teacher_id,
#                 uint32_t *students, uint16_t enrollment, room_type type,
#                 bool *availabilities, uint16_t exams_size,
#                 uint8_t max_timeslot);

init_exam = lib.init_exam
init_exam.restype = POINTER(c_structs.Exam)
init_exam.argtypes = [c_uint16, c_uint8, c_uint32, POINTER(c_uint32),
                      c_uint16, c_uint8, POINTER(c_bool), c_uint16, c_uint8]

# void free_exam(exam *e);
free_exam = lib.free_exam
free_exam.restype = None
free_exam.argtypes = [POINTER(c_structs.Exam)]

# array_exams *init_array_exams(uint16_t exams_size, exam *exams);
init_array_exams = lib.init_array_exams
init_array_exams.restype = POINTER(c_structs.ArrayExams)
init_array_exams.argtypes = [c_uint16, POINTER(POINTER(c_structs.Exam))]

# room *init_room(uint16_t id, room_type type, uint16_t capacity,
#                 uint8_t faculty, uint8_t max_timeslot);
init_room = lib.init_room
init_room.restype = POINTER(c_structs.Room)
init_room.argtypes = [c_uint16, c_uint8, c_uint16, c_uint8, c_uint8]

# array_rooms *init_array_rooms(uint16_t rooms_size, room **rooms);
init_array_rooms = lib.init_array_rooms
init_array_rooms.restype = POINTER(c_structs.ArrayRooms)
init_array_rooms.argtypes = [c_uint16, POINTER(POINTER(c_structs.Room))]

# size_t **get_rooms_sizes(uint8_t faculty_size, array_rooms *rooms);
get_rooms_sizes = lib.get_rooms_sizes
get_rooms_sizes.restype = POINTER(POINTER(c_size_t))
get_rooms_sizes.argtypes = [c_uint8, POINTER(c_structs.ArrayRooms)]

# matrix_rooms *get_rooms_matrix(uint8_t faculty_size, array_rooms *rooms,
# 								 size_t **rooms_limits);
get_rooms_matrix = lib.get_rooms_matrix
get_rooms_matrix.restype = POINTER(c_structs.MatrixRooms)
get_rooms_matrix.argtypes = [c_uint8, POINTER(c_structs.ArrayRooms),
                             POINTER(POINTER(c_size_t))]

# void compute_conflicts(array_exams const *exams);
compute_conflicts = lib.compute_conflicts
compute_conflicts.restype = None
compute_conflicts.argtypes = [POINTER(c_structs.ArrayExams)]

# bool color_graph_backtrack(array_exams *exams, matrix_rooms *rooms, uint8_t faculty_size, uint8_t max_timeslot);
color_graph_backtrack = lib.color_graph_backtrack
color_graph_backtrack.restype = c_bool
color_graph_backtrack.argtypes = [POINTER(c_structs.ArrayExams), POINTER(c_structs.MatrixRooms), c_uint8, c_uint8]

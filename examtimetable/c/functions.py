#!/usr/bin/python
# encoding: utf-8

# Python imports
import os
import ctypes
p = ctypes.POINTER

# ExamTimetable imports
import structs as c_structs

_ROOT = os.path.abspath(os.path.dirname(__file__))
libtimetable = os.path.join(_ROOT, '../..', 'lib/libtimetable')

# Load libtimetable librairy
if os.uname()[0] == 'Darwin':
    lib = ctypes.cdll.LoadLibrary(libtimetable + '.dylib')
elif os.uname()[0] == 'Linux':
    lib = ctypes.cdll.LoadLibrary(libtimetable + '.so')
else:
    raise Exception('OS not supported')

# exam *init_exam(uint16_t exam_id, uint8_t faculty, uint32_t teacher_id,
#                 uint32_t *students, uint16_t enrollment, room_type type,
#                 bool *availabilities, uint8_t max_timeslot,
#                 uint16_t exams_size, uint16_t *deps, uint8_t deps_size);

init_exam = lib.init_exam
init_exam.restype = p(c_structs.Exam)
init_exam.argtypes = [ctypes.c_uint16, ctypes.c_uint8, ctypes.c_uint32,
                      p(ctypes.c_uint32), ctypes.c_uint16, ctypes.c_uint8,
                      p(ctypes.c_bool), ctypes.c_uint8, ctypes.c_uint16,
                      p(ctypes.c_uint16), ctypes.c_uint8]

# void free_exam(exam *e);
free_exam = lib.free_exam
free_exam.restype = None
free_exam.argtypes = [p(c_structs.Exam)]

# array_exams *init_array_exams(uint16_t exams_size, exam *exams);
init_array_exams = lib.init_array_exams
init_array_exams.restype = p(c_structs.ArrayExams)
init_array_exams.argtypes = [ctypes.c_uint16, p(p(c_structs.Exam))]

# room *init_room(uint16_t id, room_type type, uint16_t capacity,
#                 uint8_t faculty, uint8_t max_timeslot);
init_room = lib.init_room
init_room.restype = p(c_structs.Room)
init_room.argtypes = [ctypes.c_uint16, ctypes.c_uint8, ctypes.c_uint16,
                      ctypes.c_uint8, ctypes.c_uint8]

# array_rooms *init_array_rooms(uint16_t rooms_size, room **rooms);
init_array_rooms = lib.init_array_rooms
init_array_rooms.restype = p(c_structs.ArrayRooms)
init_array_rooms.argtypes = [ctypes.c_uint16, p(p(c_structs.Room))]

# size_t **get_rooms_sizes(uint8_t faculty_size, uint8_t max_room_type, array_rooms *rooms);
get_rooms_sizes = lib.get_rooms_sizes
get_rooms_sizes.restype = p(p(ctypes.c_size_t))
get_rooms_sizes.argtypes = [ctypes.c_uint8, ctypes.c_uint8, p(c_structs.ArrayRooms)]

# matrix_rooms *get_rooms_matrix(uint8_t faculty_size, uint8_t max_room_type,
#                                array_rooms *rooms, size_t **rooms_limits);
get_rooms_matrix = lib.get_rooms_matrix
get_rooms_matrix.restype = p(c_structs.MatrixRooms)
get_rooms_matrix.argtypes = [ctypes.c_uint8, ctypes.c_uint8, p(c_structs.ArrayRooms),
                             p(p(ctypes.c_size_t))]

# void compute_conflicts(array_exams const *exams);
preprocess = lib.preprocess
preprocess.restype = None
preprocess.argtypes = [p(c_structs.ArrayExams)]

# bool color_graph_backtrack(array_exams *exams, matrix_rooms *rooms, uint8_t faculty_size,
#                            uint8_t max_room_type, uint8_t max_timeslot);
color_graph_backtrack = lib.color_graph_backtrack
color_graph_backtrack.restype = ctypes.c_bool
color_graph_backtrack.argtypes = [p(c_structs.ArrayExams),
                                  p(c_structs.MatrixRooms),
                                  ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8]

# void iterative_local_search(array_exams **exams, matrix_rooms **rooms,
#                             uint8_t max_timeslot, uint16_t faculty_size,
#                             uint8_t max_room_type)
iterative_local_search = lib.iterative_local_search
iterative_local_search.restype = None
iterative_local_search.argtypes = [p(p(c_structs.ArrayExams)),
                                   p(p(c_structs.MatrixRooms)),
                                   ctypes.c_uint8, ctypes.c_uint16,
                                   ctypes.c_uint8]

# float fitness_bis(array_exams *exams)
fitness_bis = lib.fitness_bis
fitness_bis.restype = ctypes.c_float
fitness_bis.argtypes = [p(c_structs.ArrayExams)]

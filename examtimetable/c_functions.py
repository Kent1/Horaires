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

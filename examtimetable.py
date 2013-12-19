#!/usr/bin/python
# encoding: utf-8

# Python imports
import json
import ctypes
p = ctypes.POINTER
# My imports
from examtimetable import c_functions, c_structs
from examtimetable.print_timetable import print_detailed_timetable


with open('datasets/example1.json') as f:
    fs = json.load(f)

# Exams

c_exams = []

for exam in fs['exams']:
    c_exam = c_functions.init_exam(
        exam['exam_id'],
        exam['faculty'],
        exam['teacher_id'],
        (ctypes.c_uint32 * len(exam['students']))(*exam['students']),
        len(exam['students']),
        exam['room_type'],
        (ctypes.c_bool * len(exam['availabilities']))(*exam['availabilities']),
        fs['max_timeslot'],
        len(fs['exams']),
        (ctypes.c_uint16 * len(exam['dependencies']))(*exam['dependencies']),
        len(exam['dependencies']),
    )
    c_exams.append(c_exam)

c_exams_p = ctypes.cast((p(c_structs.Exam) * len(c_exams))(*c_exams),
                        p(p(c_structs.Exam)))

c_array_exams = c_functions.init_array_exams(len(c_exams), c_exams_p)

# Rooms

faculty_size = 0
c_rooms = []

for room in fs['rooms']:
    faculty_size = max(faculty_size, room['faculty'])
    c_room = c_functions.init_room(
        room['room_id'],
        room['room_type'],
        room['capacity'],
        room['faculty'],
        fs['max_timeslot']
    )
    c_rooms.append(c_room)

faculty_size += 1

c_rooms_p = ctypes.cast((p(c_structs.Room) * len(c_rooms))(*c_rooms),
                        p(p(c_structs.Room)))

c_array_rooms  = c_functions.init_array_rooms(len(c_rooms), c_rooms_p)
c_rooms_sizes  = c_functions.get_rooms_sizes(faculty_size, c_array_rooms)
c_rooms_matrix = c_functions.get_rooms_matrix(faculty_size, c_array_rooms,
                                              c_rooms_sizes)

# Compute conflicts
c_functions.preprocess(c_array_exams)

if c_functions.color_graph_backtrack(c_array_exams, c_rooms_matrix,
                                     faculty_size, fs['max_timeslot']):
    print_detailed_timetable(c_array_exams, fs['max_timeslot'])

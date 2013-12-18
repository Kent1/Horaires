#!/usr/bin/python
# encoding: utf-8

# Python imports
import json
from ctypes import *

# My imports
from examtimetable import c_functions, c_structs

with open('datasets/example1.json') as f:
    fs = json.load(f)

c_exams = []

for exam in fs['exams']:
    c_exam = c_functions.init_exam(
        exam['exam_id'],
        exam['faculty'],
        exam['teacher_id'],
        (c_uint32 * len(exam['students']))(*exam['students']),
        len(exam['students']),
        exam['room_type'],
        (c_bool * len(exam['availabilities']))(*exam['availabilities']),
        len(fs['exams']),
        fs['max_timeslot']
    )
    c_exams.append(c_exam)

c_array_exams = c_functions.init_array_exams(len(c_exams),
    cast((POINTER(c_structs.Exam) * len(c_exams))(*c_exams),
         POINTER(POINTER(c_structs.Exam))))

c_rooms = []

for room in fs['rooms']:
    c_room = c_functions.init_room(
        room['room_id'],
        room['room_type'],
        room['capacity'],
        room['faculty'],
        fs['max_timeslot']
    )
    c_rooms.append(c_room)


c_array_rooms = c_functions.init_array_rooms(len(c_rooms),
    cast((POINTER(c_structs.Room) * len(c_rooms))(*c_rooms),
         POINTER(POINTER(c_structs.Room))))

print c_array_rooms.contents.data[3].contents.capacity

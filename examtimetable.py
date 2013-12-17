#!/usr/bin/python
# encoding: utf-8

# Python imports
import json
from ctypes import *

# My imports
from examtimetable import c_functions

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

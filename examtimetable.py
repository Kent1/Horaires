#!/usr/bin/python
# encoding: utf-8

# Python imports
from __future__ import print_function
import json
from ctypes import *
# My imports
from examtimetable import c_functions, c_structs

def print_detailed_schedule(c_array_exams, max_timeslot):
    print("Detailed schedule")
    print("=================")

    for i in range(max_timeslot):
        print("Timeslot %d" % (i + 1))
        print("------------\n")

        for j in range(c_array_exams.contents.size):
            if (c_array_exams.contents.data[j].contents.timeslot == i):
                print("  Exam %d :" % (j + 1))
                print("      -> Prof : %d" % c_array_exams.contents.data[j].contents.teacher_id)

                print("      -> Timeslots available : (", end="")

                for k in range(max_timeslot):
                    print("%d " % c_array_exams.contents.data[j].contents.availabilities[k], end="")

                print(")")

                print("      -> Conflicts detected : (", end="")

                for k in range(c_array_exams.contents.size):
                    print("%d " % c_array_exams.contents.data[j].contents.conflicts[k], end="")

                print(")\n")

                print("      -> Students :")

                for k in range(c_array_exams.contents.data[j].contents.enrollment):
                    print("            %d" % c_array_exams.contents.data[j].contents.students[k])

                print("      -> Room : %u" % c_array_exams.contents.data[j].contents.room_id)


with open('datasets/example1.json') as f:
    fs = json.load(f)

# Exams

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

c_array_rooms = c_functions.init_array_rooms(len(c_rooms),
    cast((POINTER(c_structs.Room) * len(c_rooms))(*c_rooms),
         POINTER(POINTER(c_structs.Room))))

c_rooms_sizes = c_functions.get_rooms_sizes(faculty_size, c_array_rooms)
c_rooms_matrix = c_functions.get_rooms_matrix(faculty_size, c_array_rooms, c_rooms_sizes)

# Compute conflicts
c_functions.compute_conflicts(c_array_exams)

if c_functions.color_graph_backtrack(c_array_exams, c_rooms_matrix, faculty_size, fs['max_timeslot']):
    print_detailed_schedule(c_array_exams, fs['max_timeslot'])

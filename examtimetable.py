#!/usr/bin/python
# encoding: utf-8

# Python imports
import argparse
import json
import ctypes
p = ctypes.POINTER
# My imports
from examtimetable import c_functions, c_structs
from examtimetable.print_timetable import print_detailed_timetable

DESC = "Generates a feasible examination timetable given examination data"


def getArgs():
    parser = argparse.ArgumentParser(description=DESC)
    parser.add_argument("file")
    return parser.parse_args()


def get_array_exams(data):
    '''
    Creates exam C structure given the data in parameters
    and returns an array_exams C structure with initialized exams.
    '''

    c_exams = []

    for exam in data['exams']:
        c_exam = c_functions.init_exam(
            exam['exam_id'],
            exam['faculty'],
            exam['teacher_id'],
            (ctypes.c_uint32 * len(exam['students']))(*exam['students']),
            len(exam['students']),
            exam['room_type'],
            (ctypes.c_bool * len(exam['availabilities']))(
                *exam['availabilities']),
            data['max_timeslot'],
            len(data['exams']),
            (ctypes.c_uint16 * len(exam['dependencies']))(
                *exam['dependencies']),
            len(exam['dependencies']),
        )
        c_exams.append(c_exam)

    c_exams_p = ctypes.cast((p(c_structs.Exam) * len(c_exams))(*c_exams),
                            p(p(c_structs.Exam)))

    return c_functions.init_array_exams(len(c_exams), c_exams_p)


def get_faculty_size(data):
    '''
    Returns the number of different faculties in the given data.
    '''

    faculty_size = 0

    for room in data['rooms']:
        faculty_size = max(faculty_size, room['faculty'])

    return faculty_size + 1


def get_rooms_matrix(data, faculty_size):
    '''
    Create room C structure given the data in parameters and
    returns rooms_matrix C structure.
    '''

    c_rooms = []

    for room in data['rooms']:
        c_room = c_functions.init_room(
            room['room_id'],
            room['room_type'],
            room['capacity'],
            room['faculty'],
            data['max_timeslot']
        )
        c_rooms.append(c_room)

    c_rooms_p = ctypes.cast((p(c_structs.Room) * len(c_rooms))(*c_rooms),
                            p(p(c_structs.Room)))

    c_array_rooms  = c_functions.init_array_rooms(len(c_rooms), c_rooms_p)
    c_rooms_sizes  = c_functions.get_rooms_sizes(faculty_size, c_array_rooms)
    c_rooms_matrix = c_functions.get_rooms_matrix(faculty_size, c_array_rooms,
                                                  c_rooms_sizes)

    return c_rooms_matrix


def main():
    args = getArgs()
    with open(args.file) as f:
        data = json.load(f)

    c_array_exams  = get_array_exams(data)
    faculty_size   = get_faculty_size(data)
    c_rooms_matrix = get_rooms_matrix(data, faculty_size)

    c_functions.preprocess(c_array_exams)

    if c_functions.color_graph_backtrack(c_array_exams, c_rooms_matrix,
                                         faculty_size, data['max_timeslot']):
        print_detailed_timetable(c_array_exams, data['max_timeslot'])


if __name__ == '__main__':
    main()

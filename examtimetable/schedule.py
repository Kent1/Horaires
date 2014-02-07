#!/usr/bin/python
# encoding: utf-8

import ctypes
p = ctypes.POINTER

from c import functions as c_fun
from c import structs as c_structs
from print_timetable import print_detailed_timetable


def get_array_exams(timeslots, exams):
    '''
    Creates exam C structure given the data in parameters
    and returns an array_exams C structure with initialized exams.
    '''

    c_exams = []

    for exam in exams:
        c_exam = c_fun.init_exam(
            exam.id,
            exam.faculty,
            exam.teacher,
            (ctypes.c_uint32 * len(exam.students))(*exam.students),
            len(exam.students),
            exam.room_type,
            (ctypes.c_bool * len(exam.availabilities))(*exam.availabilities),
            timeslots,
            len(exams),
            (ctypes.c_uint16 * len(exam.dependencies))(*exam.dependencies),
            len(exam.dependencies),
        )
        c_exams.append(c_exam)

    c_exams_p = ctypes.cast((p(c_structs.Exam) * len(c_exams))(*c_exams),
                            p(p(c_structs.Exam)))

    return c_fun.init_array_exams(len(c_exams), c_exams_p)


def get_faculty_size(rooms):
    '''
    Returns the number of different faculties in the given data.
    '''

    faculty_size = 0

    for room in rooms:
        faculty_size = max(faculty_size, room.faculty)

    return faculty_size + 1


def get_rooms_matrix(timeslots, rooms, faculty_size):
    '''
    Create room C structure given the data in parameters and
    returns rooms_matrix C structure.
    '''

    c_rooms = []

    for room in rooms:
        c_room = c_fun.init_room(
            room.id,
            room.room_type,
            room.capacity,
            room.faculty,
            timeslots
        )
        c_rooms.append(c_room)

    c_rooms_p = ctypes.cast((p(c_structs.Room) * len(c_rooms))(*c_rooms),
                            p(p(c_structs.Room)))

    c_array_rooms = c_fun.init_array_rooms(len(c_rooms), c_rooms_p)
    c_rooms_sizes = c_fun.get_rooms_sizes(faculty_size, c_array_rooms)
    c_rooms_matrix = c_fun.get_rooms_matrix(faculty_size, c_array_rooms,
                                            c_rooms_sizes)

    return c_rooms_matrix


def schedule(timeslots, exams, rooms):
    c_array_exams = get_array_exams(timeslots, exams)
    faculty_size = get_faculty_size(rooms)
    c_rooms_matrix = get_rooms_matrix(timeslots, rooms, faculty_size)

    c_fun.preprocess(c_array_exams)

    if c_fun.color_graph_backtrack(c_array_exams, c_rooms_matrix,
                                   faculty_size, timeslots):
        print_detailed_timetable(c_array_exams, timeslots)

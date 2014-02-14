#!/usr/bin/python
# encoding: utf-8

import ctypes
p = ctypes.POINTER

from c import functions as c_fun
from c import structs as c_structs


def get_array_exams(timeslots, exams):
    '''
    Creates exam C structure given the data in parameters
    and returns an array_exams C structure with initialized exams.
    '''

    c_exams = []

    for exam in exams.values():
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


def update_exams(timeslots, exams, rooms, c_array_exams):
    '''
    Update exams python objects from c_array_exams which contains
    exams C structure.
    '''

    for i in range(c_array_exams.contents.size):
        c_exam = c_array_exams.contents.data[i].contents
        exam = exams[c_exam.exam_id]
        exam.room = rooms[c_exam.room_id]
        exam.timeslot = c_exam.timeslot
        exam.conflicts = [c_exam.conflicts[i] for i in range(len(exams))]


def get_faculty_size(rooms):
    '''
    Returns the number of different faculties in the given data.
    '''

    faculty_size = 0

    for room in rooms.values():
        faculty_size = max(faculty_size, room.faculty)

    return faculty_size + 1


def get_rooms_matrix(timeslots, rooms, faculty_size):
    '''
    Create room C structure given the data in parameters and
    returns rooms_matrix C structure.
    '''

    c_rooms = []

    for room in rooms.values():
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


def update_rooms(timeslots, exams, rooms, faculty_size, room_type_size,
                 c_rooms_matrix):
    '''
    Update rooms python objects from c_rooms_matrix which contains
    rooms C structure.
    '''

    for i in range(faculty_size):
        for j in range(room_type_size):
            for k in range(c_rooms_matrix.contents.size[i][j]):
                c_room = c_rooms_matrix.contents.data[i][j][k].contents
                room = rooms[c_room.room_id]
                for l in range(timeslots):
                    room.exams[l] = []
                for l in range(timeslots):
                    room.exams[l].append(exams[c_room.assignation[l]])


def schedule(timetable):
    c_array_exams = get_array_exams(timetable.timeslots, timetable.exams)
    faculty_size = get_faculty_size(timetable.rooms)
    c_rooms_matrix = get_rooms_matrix(
        timetable.timeslots, timetable.rooms, faculty_size)

    c_fun.preprocess(c_array_exams)

    if c_fun.color_graph_backtrack(c_array_exams, c_rooms_matrix,
                                   faculty_size, timetable.timeslots):
        update_exams(timetable.timeslots, timetable.exams,
                     timetable.rooms, c_array_exams)
#!/usr/bin/python
# -*- coding: utf-8 -*-

import argparse
import json

import examtimetable


DESC = "Generates a feasible examination timetable given examination data"


def getArgs():
    parser = argparse.ArgumentParser(description=DESC)
    parser.add_argument("file")
    return parser.parse_args()


def main():
    args = getArgs()
    with open(args.file) as f:
        data = json.load(f)

    exams = {}
    for exam in data['exams']:
        temp = examtimetable.Exam(exam['id'], exam['name'], exam['faculty'],
                                  exam['professor'], exam['room_type'],
                                  exam['students'],
                                  availabilities=exam['availabilities'],
                                  dependencies=exam['dependencies'])
        exams[exam['id']] = temp

    rooms = {}
    for room in data['rooms']:
        temp = examtimetable.Room(room['id'], room['name'],
                                  room['faculty'], room['capacity'],
                                  room['room_type'])
        rooms[room['id']] = temp

    timetable = examtimetable.Timetable(data['timeslots'], exams, rooms)
    timetable.schedule()
    timetable.print_timetable()


if __name__ == '__main__':
    main()

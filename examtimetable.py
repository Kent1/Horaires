#!/usr/bin/python
# encoding: utf-8

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

    exams = []
    for exam in data['exams']:
        temp = examtimetable.Exam(exam['exam_id'], '', exam['faculty'],
                                  exam['room_type'], exam['teacher_id'],
                                  exam['students'], exam['availabilities'],
                                  exam['dependencies'])
        exams.append(temp)

    rooms = []
    for room in data['rooms']:
        temp = examtimetable.Room(room['room_id'], '', room['faculty'],
                                  room['capacity'], room['room_type'])
        rooms.append(temp)

    timetable = examtimetable.Timetable(data['max_timeslot'], exams, rooms)
    timetable.schedule()


if __name__ == '__main__':
    main()

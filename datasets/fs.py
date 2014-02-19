#!/usr/bin/python
# encoding: utf-8

import json

horaire = {}
horaire['timeslots'] = 30
horaire['exams'] = []
horaire['rooms'] = []

for i in range(4):
    exam = {
        "name"           : "coucou",
        "exam_id"        : i,
        "faculty"        : 0,
        "teacher_id"     : 555000 + i,
        "students"       : [10000, 10001, 10002],
        "room_type"      : 0,
        "availabilities" : [True] * horaire['timeslots'],
        "dependencies"   : []
    }

    horaire['exams'].append(exam)

for i in range(4):
    room = {
        "room_id"   : i,
        "room_type" : 0,
        "faculty"   : 0,
        "capacity"  : 10
    }

    horaire['rooms'].append(room)

with open('fs.json', 'w') as f:
        data = json.dump(horaire, f, sort_keys=False,
                         indent=4, separators=(',', ': '))

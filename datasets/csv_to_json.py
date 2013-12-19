#!/usr/bin/python
# encoding: utf-8

import csv


with open('fs.csv') as csvfile:
    reader = csv.reader(csvfile, delimiter=';')
    csv_read = []
    for row in reader:
        csv_read.append(row)

students = []
courses = []

for line in csv_read:
    for course in line[4:]:
        if course != '':
            courses.append(course)

# Get the id
courses_id = []
for course in courses:
    id = course.split(' - ', 1)[0]
    courses_id.append(id)

# delete duplicates
courses_id = set(courses_id)

# Fill with students
dictionary = {}
for id in courses_id:
    dictionary[id] = []


for line in csv_read:
    student = line[0]
    for course in courses_id:
        dictionary[course].append(student)

print dictionary['7465']

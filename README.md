# Examination timetable generation for University of Mons

## Introduction

The purpose of this project is to generate examination timetables for the University of Mons. The examination timetables generation is a well-known NP-Complete problem in computer sciences.
In order to solve this problem we will use heuristic algorithms. Some references are presents at the bottom of this document.

### Description of the problem

T, set of time periods (a day has two time periods, morning and afternoon)
E, set of exams
C, set of classes (a class is a group of students writing an exam e)
P, set of professors
R, set of rooms

An examination timetable generation problem consists of allocate a time period *t* for each exam *e* in E in an appropriate room *r* in *R* (in terms of capacity or features for example).
To do this, we introduce some constraints. We differentiate hard and soft constraints. The first ones can not be violated to have a feasible solution. Soft constraints can be violated, but in this case a penalty is applied.
The best solution is the solution with the lowest penalty. An optimal solution is a solution with no penalty.

### Hard constraints

 * **Student clashing**: A student can not have two or more exams in the same time period. In other words, for each exams in the same period, the intersection of classes is empty.
 * **Professor clashing**: A teacher can not be shared between two or more exams. (can be relaxed ? / in a time period)
 * **Exclusively reserved**: A room can not be shared with two or more exams. (can be relaxed ?)
 * **Full association**: Each exam *e* have to be associated with a time period *t*.
 * **Room capacity**: A room must have enough capacity to contain the class (the group of student) for associated exam *e*.
 * **Room compatibility**: A room must satisfy specified features. (Mandatory & optional features ?)
 * **Exam availability**: An exam can only be schedule in some periods. (day off ? prof availabilities ?)
 * **Room availability**: A room can only be available in some periods. (room already reserved ?)
 * **Scheduling compatibility**: Each exam may have to be scheduled before other exams (e.g. a thesis submission before its presentation)
 
### Soft constraints
 * **Uniformability**: Students should have these exams uniformally distributed
 * **Large exams**: Large exams must be in first.

## Requirements


## Compilation


## References

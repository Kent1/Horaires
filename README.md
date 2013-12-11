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
 * **Exam availability**: An exam can only be scheduled in some periods. (day off ? prof availabilities ?)
 * **Room availability**: A room can only be available in some periods. (room already reserved ?)
 * **Scheduling compatibility**: Each exam may have to be scheduled before other exams (e.g. a thesis submission before its presentation)

### Soft constraints
 * **Uniformability**: Students should have these exams uniformally distributed
 * **Large exams**: Large exams must be in first.

### Construction of the timetable

First, we need to construct a feasible timetable (i.e. a timetable which does not violate any hard constraints). To do this, we transform the examination timetable problem to a coloring graph problem. The vertex of the graph are the exams and the edges are hard constraints. For example, two exams with students in common are linked with an edge.

For resolve the coloring problem, we use ordering heuristics. We considered three ordering heuristics which are :

 * **Largest Degree** : Exams are ranked in descending order by the number of exams in conflict -- i.e. priority is given to exams with the greatest number of exams in conflict.
 * **Largest Enrollment** : Exams are ranked in descending order by the number of students enrolled in each of the exam -- i.e. exams with the highest number of students are given the highest priority.
 * **Saturation Degree** : Exams are ranked in increasing order by the number of valid timeslots remaining in the timetable for each exam -- priority is given to exams with fewer time slots available.

We decided to use Saturation Degree for our principal heuristic because its fits well with our hard constraints (especially with exam avaibility). We also decided to use Largest Enrollment in last resort (in the case of ex-aequo).

For the room-type constraints, after computing the feasible timetable for the timeslot, we try to assigned a room to all scheduled exams (and all exams are scheduled). For the assignement, for each exam, we consider each room of desired type. If a room of the right type is available and it has enough capacity, we assign this room to the exam. If no solution is found, we do backtrack on the timeslot assignement.

### Data structures

#### Struct : exam
 * exam_id (uint16_t)
 * teacher_id (uint32_t)
 * timeslot (uint8_t)              // assigned timeslot
 * enrollment (uint16_t)        // number of students, also sizeof students
 * availabilities (uint8_t*)      // bool array for the availabilities
 * dependancies (uint16_t*) // list of prerequisites
 * students (uint32_t*)         // list of students enrolled in the event
 * conflicts (uint16_t*)          // bool array describing the list of conflicting exams
 * room_type room_type;     // Room type needed
 * uint16_t room_id;             // Room id assigned

#### Struct : room
 * room_id (uint16_t)
 * type (enum room_type) // Type of the room (Amphitheater, computer room)
 * capacity (uint16_t)         // Number of seats
 * faculty (uint8_t)              // Faculty which owns the room
 * assignation (uint16_t*)  // contains for each timeslot the assigned exam_id


## Requirements


## Compilation


## References

[1] Asmuni, Hishammudin and Burke, Edmund K and Garibaldi, Jonathan M and McCollum, Barry and Parkes, Andrew J, *An investigation of fuzzy multiple heuristic orderings in the construction of university examination timetables*, Computers & Operations Research 2009

[2] Qu, Rong and Burke, EK and McCollum, Barry and Merlot, Liam TG and Lee, Sau Y, *A survey of search methodologies and automated approaches for examination timetabling*, Computer Science Technical Report No. NOTTCS-TR-2006-4, UK, 2006

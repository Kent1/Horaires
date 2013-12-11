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

Constraints & feasible timetable construction are described in the lib/ directory. It also contains the code source for the feasible timetable construction.

## Requirements



## Compilation

You can compile the code using the Makefile in the root of this directory. This Makefile compiles the source included in lib/ directory.

## References

[1] Asmuni, Hishammudin and Burke, Edmund K and Garibaldi, Jonathan M and McCollum, Barry and Parkes, Andrew J, *An investigation of fuzzy multiple heuristic orderings in the construction of university examination timetables*, Computers & Operations Research 2009

[2] Qu, Rong and Burke, EK and McCollum, Barry and Merlot, Liam TG and Lee, Sau Y, *A survey of search methodologies and automated approaches for examination timetabling*, Computer Science Technical Report No. NOTTCS-TR-2006-4, UK, 2006

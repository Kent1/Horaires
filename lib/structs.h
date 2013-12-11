/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 28/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

/* Constants */
#define MAX_ROOM_TYPE 4

/* Defines an enumeration type for different room types */
typedef enum {
    classroom,
    lab,
    computer_room,
} room_type;


/* Defines a type struct for rooms */
typedef struct {
    uint16_t  room_id;
    room_type type;         // Type of the room (Amphitheater, lab, ...)
    uint8_t   faculty;      // Faculty which owns the room
    uint16_t  capacity;     // Number of seats
    uint16_t  *assignation; // Contains for each timeslot the assigned exam_id
} room;


/* Defines a type struct for exams */
typedef struct {
    uint16_t exam_id;
    uint8_t  faculty;    // Exams are associated in general to a faculty
    uint32_t teacher_id; // ID of the teacher responsible of the exam

    uint8_t  timeslot;   // Assigned timeslot
    uint16_t enrollment; // Number of students, also sizeof students

    uint16_t *deps;      // List of prerequisites
    uint32_t *students;  // List of students enrolled in the event

    // Bool array, describing a list of all timeslots in which
    // the i-th index is true if the exam can be sheduled at
    // the i-th timeslot, false otherwhise
    bool *availabilities;

    // Bool array, describing a list of adjacencies for conflicting
    // exams in which true means there is a conflict with the i-th exam,
    // false otherwhise
    bool *conflicts;

    // Room informations
    uint16_t  room_id;   // Room id assigned
    room_type room_type; // Room type needed
} exam;



#endif /*STRUCTS_H_*/

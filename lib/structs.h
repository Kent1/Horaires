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
#define MAX_ROOM_TYPE 3

/**
 * @enum room_type
 *
 * enumeration type for the different room types.
 */
typedef enum {
    classroom,
    lab,
    computer_room,
} room_type;


/**
 * @struct room
 * @brief This structure represents a room.
 *
 * @var room::room_id
 * Id of the room.
 *
 * @var room::type
 * Type of the room (Amphitheater, lab, ...). @see room_type.
 *
 * @var room::faculty
 * Faculty which owns the room.
 *
 * @var room::capacity
 * Number of seats in the room.
 *
 * @var room::assignation
 * Array representing the reservation of the room.
 * Contains for each timeslot the assigned exam_id.
 */
typedef struct {
    uint16_t  const room_id;
    room_type const type;
    uint8_t   const faculty;
    uint16_t  const capacity;
    uint16_t  *assignation;
} room;


/**
 * @struct exam
 * @brief This structure represents an exam.
 *
 * @var exam::exam_id
 * Id of the exam.
 *
 * @var exam::faculty
 * Exams are associated in general to a faculty.
 *
 * @var exam::teacher_id
 * ID of the teacher responsible of the exam.
 *
 * @var exam::students
 * List of students which have to take the exam.
 *
 * @var exam::enrollment
 * Number of students, also sizeof students.
 *
 * @var exam::room_type
 * Needed room_type for this exam.
 *
 * @var exam::room_id
 * Assigned room id to this exam.
 *
 * @var exam::timeslot
 * Assigned timeslot.
 *
 * @var exam::availabilities
 * Bool array, describing a list of all timeslots in which the i-th index
 * is true if the exam can be sheduled at the i-th timeslot, false otherwhise.
 *
 * @var exam::conflicts
 * Bool array, describing a list of adjacencies for conflicting exams in
 * which true means there is a conflict with the i-th exam, false otherwhise.
 *
 * @var exam::deps
 * List of prerequisites.
 */
typedef struct {
    uint16_t  const exam_id;

    uint8_t   const faculty;

    uint32_t  const teacher_id;

    uint32_t  const *students;
    uint16_t  const enrollment;

    room_type const room_type;
    uint16_t room_id;

    uint8_t timeslot;

    bool *availabilities;

    bool *conflicts;
    uint16_t *deps;
} exam;


#endif /*STRUCTS_H_*/

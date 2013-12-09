/*
 * This file is part of UMons - horaires project
 *
 *	Created on: 28/11/2013
 *		Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef EXAM_H_
#define EXAM_H_

#include "../util.h"

typedef struct {
	uint16_t exam_id;
	uint32_t teacher_id;

	uint8_t timeslot; // assigned timeslot
	uint16_t enrollment; // number of students, also sizeof students

    // Bool array, describing a list of all timeslots in which
    // the i-th index is true if the exam can be sheduled at
    // the i-th timeslot, false otherwhise
	uint8_t* availabilities;
	uint16_t* deps; // list of prerequisites
	uint32_t* students; // list of students enrolled in the event

    // Bool array, describing a list of adjacencies for conflicting
    // exams in which true means there is a conflict with the i-th exam,
    // false otherwhise
	uint16_t* conflicts;
    room_type room_type; // Room type needed
    uint16_t room_id; // Room id assigned
} exam;

#endif /*EXAM_H_*/

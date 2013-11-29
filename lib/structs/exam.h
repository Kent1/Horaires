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

	uint8_t* unavailabilities; // list of unavailabilities
	uint16_t* deps; // list of prerequisites
	uint32_t* students; // list of students enrolled in the event

	uint16_t* conflicts; // list of events in conflicts with this
} exam;

#endif /*EXAM_H_*/

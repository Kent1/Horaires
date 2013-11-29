/*
 * This file is part of Umons - horaires project
 *
 *	Created on: 28/11/2013
 *		Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "conflict.h"

/**
 * Compute conflict between two exams. The exam->students array
 * MUST be sorted.
 *
 * @param exam1 The first exam
 * @param exam2 The second exam
 * @return true if the two exams are in conflict, false otherwise.
 */
bool compute_conflict(exam* exam1, exam* exam2) {
	uint32_t i = 0, j = 0;
	while(i < exam1->enrollment && j < exam2->enrollment) {
		if(exam1->students[i] < exam2->students[j])
			i++;
		else if(exam2->students[j] < exam1->students[i])
			j++;
		else /* exam1->students[i] == exam2->students[j] */
		{
			return true;
		}
	}
	return false;
}

/**
 * Given an array of exams, computes all of the conflicts between
 * each exam & fills the exam->conflicts array
 *
 * @param exams Array of exams
 * @param nbr_exam Size of the array
 */
void compute_conflicts(exam* exams, uint16_t nbr_exam) {
	uint16_t i, j;
	for (i = 0; i < nbr_exam; i++) {
		exam* exam1 = &exams[i];
		// Exam is not in conflict with itself
		exam1->conflicts[exam1->exam_id] = 0;
		for (j = i+1; j < nbr_exam; j++) {
			exam* exam2 = &exams[j];
			if(compute_conflict(exam1, exam2)) {
				// If the two exams are in conflict
				// i.e. there are at least one student in common
				exam1->conflicts[exam2->exam_id] = 1;
				exam2->conflicts[exam1->exam_id] = 1;
			}
			else {
				// Else, they are not in conflict
				exam1->conflicts[exam2->exam_id] = 0;
				exam2->conflicts[exam1->exam_id] = 0;
			}
		}
	}
}

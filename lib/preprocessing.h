/*
 * This file is part of Umons - horaires project
 *
 *  Created on: 28/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef CONFLICT_H_
#define CONFLICT_H_

/**
 * Compute conflict between two exams. The exam->students array
 * MUST be sorted. There is a conflict when teacher is the same
 * for the two exams or when exams have students in common.
 *
 * @param exam1 The first exam.
 * @param exam2 The second exam.
 * @return true if the two exams are in conflict, false otherwise.
 */
bool compute_conflict(exam const *exam1, exam const *exam2);

/**
 * Given an array of exams, computes all of the conflicts between
 * each exam & fills the exam->conflicts array.
 *
 * @param exams Array of exams (struct array_exams).
 */
void compute_conflicts(array_exams const *exams);

/**
 * Replaces exam_id included in deps field by index id in array_exams.
 *
 * @param exams Array of exams (struct array_exams).
 */
void compute_deps(array_exams const *exams);

/**
 * Preprocesses the exams. Computes conflicts & replaces exam_id in deps.
 *
 * @param exams Array of exams (struct array_exams).
 */
void preprocess(array_exams const *exams);

#endif /*CONFLICT_H_*/

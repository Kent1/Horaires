/*
 * This file is part of Umons - horaires project
 *
 *	Created on: 28/11/2013
 *		Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef CONFLICT_H_
#define CONFLICT_H_

#include "structs/exam.h"

bool compute_conflict(exam* exam1, exam* exam2);
void compute_conflicts(exam* exams, ushort nbr_exam);

#endif /*CONFLICT_H_*/

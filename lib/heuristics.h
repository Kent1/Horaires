/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 28/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef HEURISTICS_H_
#define HEURISTICS_H_

#include "util.h"
#include "structs/exam.h"

exam* get_first_exam(exam*, uint16_t);

uint8_t* get_exams_saturation_degree(exam*, uint16_t);

#endif /*HEURISTICS_H_*/
